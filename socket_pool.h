#ifndef SOCKET_POOL_H
#define SOCKET_POOL_H

#define MAX_BYTES 4

void* connection_pool_handler(int*);
int setup_socket_pool(unsigned int, unsigned int, const char*);

#endif //SOCKET_POOL_H