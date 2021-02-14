#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "service_manager.h"

unsigned int counter_manager = 1;
unsigned volatile int pos_current = 0;

pthread_mutex_t mutex_service_queue = PTHREAD_MUTEX_INITIALIZER;

service_manager_t* create_manager(void) {
    service_manager_t* services = malloc(sizeof(service_manager_t)*BASE_SERVICE_LENGTH);
    memset(services, 0, sizeof(service_manager_t)*BASE_SERVICE_LENGTH);

    return services;
}

void clear_manager(service_manager_t* services) {
    free(services);
}

unsigned int create_service(service_manager_t* services, void* (*fn)(void*)) {
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

int execute_service(service_manager_t* services, unsigned int key, void* args) {
    if (key > (BASE_SERVICE_LENGTH*counter_manager)) {
        return -1;
    }

    if (services[key].fn != NULL) {
        pthread_mutex_lock(&mutex_service_queue);
        services[key].fn(args);
        pthread_mutex_unlock(&mutex_service_queue);
        return 1;
    }

    return -1;
}