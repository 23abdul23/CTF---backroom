#include "recv_thread.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "../config.h"
#include "../core/game.h"
#include "net.h"
#include "peer_manager.h"

static volatile int g_recv_running = 0;

static void *recv_thread_main(void *arg) {
    (void)arg;
    g_recv_running = 1;

    while (g_recv_running && g_game.running) {
        fd_set set;
        FD_ZERO(&set);

        int maxfd = -1;
        for (int pid = 0; pid < MAX_PLAYERS; ++pid) {
            if (pid == g_game.local_player_id) {
                continue;
            }
            int sock = peer_manager_get_socket(pid);
            if (sock >= 0) {
                FD_SET(sock, &set);
                if (sock > maxfd) {
                    maxfd = sock;
                }
            }
        }

        if (maxfd < 0) {
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = RECV_THREAD_TIMEOUT_USEC;
            select(0, NULL, NULL, NULL, &tv);
            continue;
        }

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = RECV_THREAD_TIMEOUT_USEC;

        int ready = select(maxfd + 1, &set, NULL, NULL, &tv);
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("select");
            break;
        }
        if (ready == 0) {
            continue;
        }

        for (int pid = 0; pid < MAX_PLAYERS; ++pid) {
            if (pid == g_game.local_player_id) {
                continue;
            }
            int sock = peer_manager_get_socket(pid);
            if (sock < 0 || !FD_ISSET(sock, &set)) {
                continue;
            }

            /* Receive player position updates */
            PlayerUpdate update;
            if (recv_all(sock, &update, sizeof(update)) != 0) {
                fprintf(stderr, "Peer %d disconnected\n", pid);
                close(sock);
                peer_manager_set_socket(pid, -1);
                continue;
            }
            
            /* Update remote player position */
            game_update_player_position(update.player_id, update.x, update.y, update.angle);
        }
    }

    g_recv_running = 0;
    return NULL;
}

int recv_thread_start(pthread_t *thread) {
    return pthread_create(thread, NULL, recv_thread_main, NULL);
}

void recv_thread_stop(void) {
    g_recv_running = 0;
}