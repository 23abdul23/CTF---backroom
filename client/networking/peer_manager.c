#include "peer_manager.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../config.h"
#include "net.h"

static int g_local_player_id = -1;
static int g_peer_sockets[MAX_PLAYERS] = {-1, -1, -1, -1};
pthread_mutex_t g_peer_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_left(int id) {
    switch (id) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 0;
        case 3: return 1;
        default: return -1;
    }
}

int get_right(int id) {
    switch (id) {
        case 0: return 2;
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        default: return -1;
    }
}

int get_top(int id) {
    switch (id) {
        case 0: return 3;
        case 1: return 3;
        case 2: return 3;
        case 3: return 0;
        default: return -1;
    }
}

int get_bottom(int id) {
    switch (id) {
        case 0: return 3;
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        default: return -1;
    }
}

void peer_manager_init(int local_player_id) {
    pthread_mutex_lock(&g_peer_mutex);
    g_local_player_id = local_player_id;
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        g_peer_sockets[i] = -1;
    }
    pthread_mutex_unlock(&g_peer_mutex);
}

void peer_manager_set_socket(int player_id, int sock) {
    pthread_mutex_lock(&g_peer_mutex);
    g_peer_sockets[player_id] = sock;
    pthread_mutex_unlock(&g_peer_mutex);
}

int peer_manager_get_socket(int player_id) {
    int sock;
    pthread_mutex_lock(&g_peer_mutex);
    sock = g_peer_sockets[player_id];
    pthread_mutex_unlock(&g_peer_mutex);
    return sock;
}

void peer_manager_shutdown(void) {
    pthread_mutex_lock(&g_peer_mutex);
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (g_peer_sockets[i] >= 0) {
            close(g_peer_sockets[i]);
            g_peer_sockets[i] = -1;
        }
    }
    pthread_mutex_unlock(&g_peer_mutex);
}

static int connect_to_peer(const PlayerInfo *pinfo) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(pinfo->port);
    if (inet_pton(AF_INET, pinfo->ip, &addr.sin_addr) <= 0) {
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }

    PeerHello hello;
    hello.player_id = g_local_player_id;
    if (send_all(sock, &hello, sizeof(hello)) != 0) {
        close(sock);
        return -1;
    }
    return sock;
}

int peer_manager_connect_mesh(const JoinResponse *join_info, int listen_sock) {
    /* Connect to lower-numbered players */
    for (int pid = 0; pid < g_local_player_id; ++pid) {
        /* Skip if player has no valid IP (not in game) */
        if (join_info->players[pid].ip[0] == '\0') {
            continue;
        }
        
        int sock = -1;
        for (int attempts = 0; attempts < 100 && sock < 0; ++attempts) {
            sock = connect_to_peer(&join_info->players[pid]);
            if (sock < 0) {
                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 100000;
                select(0, NULL, NULL, NULL, &tv);
            }
        }
        if (sock < 0) {
            fprintf(stderr, "Could not connect to peer %d\n", pid);
            return -1;
        }
        peer_manager_set_socket(pid, sock);
        printf("Connected to Player %d\n", pid);
    }

    /* Accept connections from higher-numbered players */
    for (int pid = g_local_player_id + 1; pid < MAX_PLAYERS; ++pid) {
        /* Skip if player has no valid IP (not in game) */
        if (join_info->players[pid].ip[0] == '\0') {
            continue;
        }
        
        struct sockaddr_in caddr;
        socklen_t clen = sizeof(caddr);
        int sock = accept(listen_sock, (struct sockaddr *)&caddr, &clen);
        if (sock < 0) {
            perror("accept peer");
            return -1;
        }

        PeerHello hello;
        if (recv_all(sock, &hello, sizeof(hello)) != 0) {
            close(sock);
            return -1;
        }

        peer_manager_set_socket(hello.player_id, sock);
        printf("Connected to Player %d\n", hello.player_id);
    }

    return 0;
}