#include <string.h>
#include "key_manager.h"
#include "socket_pool.h"

void get_user(int* client, char* data) {
    char response[20000];
    memset(response, 'C', 2000);
    send_response(client, response);
}

int main() {
    create_manager();
    const char* default_key = create_key_bucket();

    int ret = create_service("user/find", (void *(*)(int *, void *)) &get_user);
    attach_service_to_key(ret, default_key);

    ret = create_service("user/create", (void *(*)(int *, void *)) &get_user);
    attach_service_to_key(ret, default_key);

    ret = setup_socket_thread_pool(8888, 2, "127.0.0.1");

    return 0;
}