#include "socket_pool.h"

int main() {
    service_manager_t* services = create_manager();
    int response = setup_socket_thread_pool(8888, 2, "127.0.0.1", services);

    return 0;
}