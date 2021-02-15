#include <string.h>
#include "socket_pool.h"

void get_user(int* client, char* data) {
    char response[20000];
    memset(response, 'C', 2000);
    send_response(client, response);
}

int main() {
    create_manager();
    int ret = create_service("user/find", (void *(*)(int *, void *)) &get_user);

    if (ret < 0) {
        return 1;
    }

    ret = setup_socket_thread_pool(8888, 2, "127.0.0.1");

    return 0;
}