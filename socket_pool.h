#ifndef SOCKET_POOL_H
#define SOCKET_POOL_H
#include "service_manager.h"

#define MAX_BYTES 4

void* connection_pool_handler(int*);
int setup_socket_thread_pool(unsigned int, unsigned int, const char*, service_manager_t*);

#endif //SOCKET_POOL_H