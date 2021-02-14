#ifndef H_CONNECTION_H
#define H_CONNECTION_H

#include "socket_pool.h"

char** read_header(char**, char *);
int handle_connection(int, int*);

#endif //H_CONNECTION_H