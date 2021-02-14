#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "service_manager.h"

service_manager_t* services;
unsigned int counter_manager = 1;
unsigned volatile int pos_current = 0;

pthread_mutex_t mutex_service_queue = PTHREAD_MUTEX_INITIALIZER;

void create_manager(void) {
    pthread_mutex_lock(&mutex_service_queue);

    services = malloc(sizeof(service_manager_t)*BASE_SERVICE_LENGTH);
    memset(services, 0, sizeof(service_manager_t)*BASE_SERVICE_LENGTH);

    pthread_mutex_unlock(&mutex_service_queue);
}

void clear_manager(void) {
    free(services);
}

unsigned int create_service(void* (*fn)(void*)) {
    pthread_mutex_lock(&mutex_service_queue);
    if (services[(BASE_SERVICE_LENGTH*counter_manager)-1].fn != NULL) {
        counter_manager+=1;

        services = realloc(services, sizeof(service_manager_t)*(BASE_SERVICE_LENGTH*counter_manager));
        memset(services+BASE_SERVICE_LENGTH,0,BASE_SERVICE_LENGTH);
    }

    services[pos_current].key = pos_current;
    services[pos_current].fn = fn;

    unsigned int response = pos_current+=1;
    pthread_mutex_unlock(&mutex_service_queue);

    return response - 1;
}

int execute_service(unsigned int key, void* args) {
    if (key > (BASE_SERVICE_LENGTH*counter_manager)) {
        return -1;
    }

    if (services[key].fn != NULL) {
        services[key].fn(args);
        return 1;
    }

    return -1;
}