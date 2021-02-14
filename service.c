#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include "service.h"

unsigned int counter_manager = 1;
unsigned volatile int pos_current = 0;

pthread_mutex_t mutex_service_queue = PTHREAD_MUTEX_INITIALIZER;

void create_manager(void) {
    services = malloc(sizeof(service_t) * BASE_SERVICE_LENGTH);
    memset(services, 0, sizeof(service_t) * BASE_SERVICE_LENGTH);
}

void clear_manager(void) {
    pthread_mutex_lock(&mutex_service_queue);
    free(services);
    pthread_mutex_unlock(&mutex_service_queue);
}

unsigned int create_service(char* service_name, void* (*fn)(int*,void*)) {
    pthread_mutex_lock(&mutex_service_queue);
    if (services[(BASE_SERVICE_LENGTH*counter_manager)-1].fn != NULL) {
        counter_manager+=1;

        services = realloc(services, sizeof(service_t) * (BASE_SERVICE_LENGTH * counter_manager));
        memset(services+BASE_SERVICE_LENGTH, 0, sizeof(service_t) * BASE_SERVICE_LENGTH);
    }

    services[pos_current].key = pos_current;
    strcpy(services[pos_current].service_name, service_name);
    services[pos_current].fn = fn;

    unsigned int response = pos_current+=1;
    pthread_mutex_unlock(&mutex_service_queue);

    return response - 1;
}

int send_response(const int* client, const char* response) {
    if (write(*client, response, strlen(response)+1) < 0) {
        return -1;
    }

    return 0;
}

int execute_service(unsigned int key, int* client, char* args) {
    if (key > (BASE_SERVICE_LENGTH*counter_manager)) {
        return -1;
    }

    if (services[key].fn != NULL) {
        pthread_mutex_lock(&mutex_service_queue);
        services[key].fn(client, args);
        pthread_mutex_unlock(&mutex_service_queue);
        return 1;
    }

    return -1;
}