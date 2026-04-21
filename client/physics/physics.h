#ifndef PHYSICS_H
#define PHYSICS_H

#include "../../common/protocol.h"

void physics_update_ball(Ball *ball);
void physics_handle_ball_collisions(Ball *balls, int ball_count, int local_player_id);

#endif