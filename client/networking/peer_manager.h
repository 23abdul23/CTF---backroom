#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H

#include <pthread.h>

#include "../../common/protocol.h"

void peer_manager_init(int local_player_id);
void peer_manager_shutdown(void);

void peer_manager_set_socket(int player_id, int sock);
int peer_manager_get_socket(int player_id);

int peer_manager_connect_mesh(const JoinResponse *join_info, int listen_sock);

int get_left(int id);
int get_right(int id);
int get_top(int id);
int get_bottom(int id);

extern pthread_mutex_t g_peer_mutex;

#endif