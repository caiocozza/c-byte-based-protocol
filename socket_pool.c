#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "handle_connection.h"
#include "socket_pool.h"
#include "socket_queue.h"

pthread_mutex_t mutex_socket_queue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_socket_queue = PTHREAD_COND_INITIALIZER;

_Noreturn void* connection_pool_handler(int* thread_id) {
    printf("Thread #%d created\n", *thread_id);

    while(1) {
        int* client;

        pthread_mutex_lock(&mutex_socket_queue);
        if ((client = dequeue()) == NULL) {
            pthread_cond_wait(&cond_socket_queue, &mutex_socket_queue);
            client = dequeue();
        }
        pthread_mutex_unlock(&mutex_socket_queue);

        if (client != NULL) {
            handle_connection(MAX_BYTES, client);
        }
    }

    free(thread_id);
}

int setup_socket_thread_pool(const unsigned int port, const unsigned int length_pool, const char* ip, service_manager_t* services) {
    struct sockaddr_in address;
    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    if (listenfd < 0) {
        return -1;
    }

    if (bind(listenfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        return -1;
    }

    if (listen(listenfd, 100000) < 0) {
        return -1;
    }

    pthread_t threads[length_pool];

    for(int i = 0; i < length_pool; i++) {
        int* thread_p = malloc(sizeof(int));
        *thread_p = i;
        if (pthread_create(&threads[i], NULL, (void*)connection_pool_handler, thread_p)) {
            return -1;
        }

        pthread_detach(threads[i]);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);

        int connfd = accept(listenfd, ( struct sockaddr* )&client_addr, &client_addrlen);
        
        if (connfd < 0) {
            // TODO: proper handle this warning
            continue;
        }

        pthread_mutex_lock(&mutex_socket_queue);
        int* client = malloc(sizeof(int));
        *client = connfd;

        enqueue(client);
        pthread_mutex_unlock(&mutex_socket_queue);
        pthread_cond_signal(&cond_socket_queue);
    }

    return 0;
}