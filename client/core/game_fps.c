#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "../simulation/level.h"
#include "../input/input.h"
#include "../networking/peer_manager.h"

GameState g_game;

void game_init(int local_player_id, const JoinResponse *join_info) {
    g_game.local_player_id = local_player_id;
    memcpy(&g_game.join_info, join_info, sizeof(*join_info));
    
    pthread_mutex_init(&g_game.player_mutex, NULL);
    
    /* Initialize player positions */
    for (int i = 0; i < MAX_PLAYERS; i++) {
        g_game.players[i].x = 5.0f + i * 3.0f;
        g_game.players[i].y = 5.0f + i * 2.0f;
        g_game.players[i].angle = 0.0f;
        g_game.players[i].fov = 1.047f; /* 60 degrees */
    }
    
    /* Local player starts in corner */
    g_game.local_player.x = 5.0f;
    g_game.local_player.y = 5.0f;
    g_game.local_player.angle = 0.0f;
    g_game.local_player.fov = 1.047f;
    
    g_game.running = 1;
    
    printf("Game initialized for player %d\n", local_player_id);
}

void game_shutdown(void) {
    g_game.running = 0;
    pthread_mutex_destroy(&g_game.player_mutex);
}

void game_update_step(void) {
    /* Get input and update local player */
    PlayerInput *input = input_get_current();
    
    /* Movement */
    float move_speed = 0.1f;
    if (input->forward) {
        g_game.local_player.x += cosf(g_game.local_player.angle) * move_speed;
        g_game.local_player.y += sinf(g_game.local_player.angle) * move_speed;
    }
    if (input->backward) {
        g_game.local_player.x -= cosf(g_game.local_player.angle) * move_speed;
        g_game.local_player.y -= sinf(g_game.local_player.angle) * move_speed;
    }
    if (input->strafe_left) {
        g_game.local_player.x += cosf(g_game.local_player.angle - 1.57f) * move_speed;
        g_game.local_player.y += sinf(g_game.local_player.angle - 1.57f) * move_speed;
    }
    if (input->strafe_right) {
        g_game.local_player.x += cosf(g_game.local_player.angle + 1.57f) * move_speed;
        g_game.local_player.y += sinf(g_game.local_player.angle + 1.57f) * move_speed;
    }
    
    /* Rotation */
    float rotate_speed = 0.00f;
    if (input->turn_left) {
        g_game.local_player.angle -= rotate_speed;
    }
    if (input->turn_right) {
        g_game.local_player.angle += rotate_speed;
    }
    
    /* Clamp to valid position */
    if (g_game.local_player.x < 1.0f) g_game.local_player.x = 1.0f;
    if (g_game.local_player.x > LEVEL_WIDTH - 1.0f) g_game.local_player.x = LEVEL_WIDTH - 1.0f;
    if (g_game.local_player.y < 1.0f) g_game.local_player.y = 1.0f;
    if (g_game.local_player.y > LEVEL_HEIGHT - 1.0f) g_game.local_player.y = LEVEL_HEIGHT - 1.0f;
    
    /* TODO: Broadcast player position to other players */
}

void game_update_player_position(int player_id, float x, float y, float angle) {
    pthread_mutex_lock(&g_game.player_mutex);
    if (player_id >= 0 && player_id < MAX_PLAYERS) {
        g_game.players[player_id].x = x;
        g_game.players[player_id].y = y;
        g_game.players[player_id].angle = angle;
    }
    pthread_mutex_unlock(&g_game.player_mutex);
}

int game_is_local_region(float x, float y, int player_id) {
    return 1;
}
