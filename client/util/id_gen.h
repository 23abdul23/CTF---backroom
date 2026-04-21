#ifndef ID_GEN_H
#define ID_GEN_H

static inline int make_initial_ball_id(int player_id) {
    return player_id * 1000;
}

#endif