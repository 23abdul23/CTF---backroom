#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#define WIDTH 1000
#define HEIGHT 700
#define BALL_RADIUS 20.0f

#define DT 0.016f
#define MAX_SPEED 350.0f
#define RESTITUTION 0.0f

#define MAX_PLAYERS 6
#define MAX_BALLS 4
#define MAX_SEEN_IDS 512

#define INITIAL_BALLS_PER_PLAYER 1

#define BALL_COLOR_COUNT 8
#define BALL_COLOR_0_R 0.95f
#define BALL_COLOR_0_G 0.30f
#define BALL_COLOR_0_B 0.25f
#define BALL_COLOR_1_R 0.25f
#define BALL_COLOR_1_G 0.70f
#define BALL_COLOR_1_B 0.95f
#define BALL_COLOR_2_R 0.25f
#define BALL_COLOR_2_G 0.90f
#define BALL_COLOR_2_B 0.45f
#define BALL_COLOR_3_R 0.98f
#define BALL_COLOR_3_G 0.72f
#define BALL_COLOR_3_B 0.18f
#define BALL_COLOR_4_R 0.85f
#define BALL_COLOR_4_G 0.34f
#define BALL_COLOR_4_B 0.92f
#define BALL_COLOR_5_R 0.98f
#define BALL_COLOR_5_G 0.88f
#define BALL_COLOR_5_B 0.20f
#define BALL_COLOR_6_R 0.28f
#define BALL_COLOR_6_G 0.78f
#define BALL_COLOR_6_B 0.78f
#define BALL_COLOR_7_R 0.90f
#define BALL_COLOR_7_G 0.50f
#define BALL_COLOR_7_B 0.22f

#define DRAG_THROW_VEL_SCALE 1.15f
#define DRAG_THROW_ACCEL_SCALE 1.8f

#define BALL_PICKUP_EXTRA_RADIUS 8.0f
#define STOP_VELOCITY_EPSILON 1.5f

#define SERVER_PORT 5000
#define RECV_THREAD_TIMEOUT_USEC 100000

#endif