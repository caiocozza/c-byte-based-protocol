#include "socket_pool.h"

int main() {
    int response = setup_socket_pool(8888, 2, "127.0.0.1");

    return 0;
}