#ifndef SOCKET_POOL_H
#define SOCKET_POOL_H
#include "service.h"

#define MAX_BYTES 1024

static unsigned char listening = 0;

void* connection_pool_handler(int*);
int setup_socket_thread_pool(unsigned int, unsigned int, const char*);

#endif //SOCKET_POOL_H