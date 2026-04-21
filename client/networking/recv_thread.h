#ifndef RECV_THREAD_H
#define RECV_THREAD_H

#include <pthread.h>

int recv_thread_start(pthread_t *thread);
void recv_thread_stop(void);

#endif