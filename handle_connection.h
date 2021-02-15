#ifndef H_CONNECTION_H
#define H_CONNECTION_H

#include "socket_pool.h"
#include "errors.h"

int terminate_request_with_error(int*, errors);
int terminate_request(int*);
char** read_header(char**, char *);
int handle_connection(int, int*);

#endif //H_CONNECTION_H