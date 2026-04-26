#ifndef SERVER_H
#define SERVER_H

#define SERVER_PORT 5000
#define MAX_PLAYERS 6
#define MIN_PLAYERS_TO_START 2  /* Start game once at least 2 players are connected */

int run_server(void);

#endif