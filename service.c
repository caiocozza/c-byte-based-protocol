#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "service.h"
#include "errors.h"
#include "socket_pool.h"

unsigned int counter_manager = 1;
unsigned volatile int pos_current = 0;

pthread_mutex_t mutex_service_queue = PTHREAD_MUTEX_INITIALIZER;

void create_manager(void) {
    if (listening == 1) {
        return; // nothing for now
    }

    services = malloc(sizeof(service_t) * BASE_SERVICE_LENGTH);
    memset(services, 0, sizeof(service_t) * BASE_SERVICE_LENGTH);
}

void clear_manager(void) {
    if (listening == 1) {
        return; // nothing for now
    }

    pthread_mutex_lock(&mutex_service_queue);
    free(services);
    pthread_mutex_unlock(&mutex_service_queue);
}

int get_services(int*client, char* key) {
    // We are going to deliver services based on the key provided. If has access to it, for now return all
    char* str_response = malloc(pos_current*256);
    for(int i=0;i<pos_current;i++) {
        strcat(str_response, services[i].service_name);
        strcat(str_response, "\n");
    }
    strcat(str_response, "\r\r");
    if (write(*client, str_response, strlen(str_response)) < 0) {
        return -1;
    }

    free(str_response);

    return 0;
}

int service_exists(const unsigned int key) {
    if (services[key].fn == NULL || services[key].key != key) {
        return -1;
    }

    return 0;
}

int create_service(char* service_name, void* (*fn)(int*,void*)) {
    if (listening == 1) {
        return -1;
    }

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
    char* response_with_end = malloc(strlen(response)+2);
    strcpy(response_with_end, response);
    strcat(response_with_end, "\r\n");
    if (write(*client, response_with_end, strlen(response_with_end)) < 0) {
        return -1;
    }

    free(response_with_end);
    return 0;
}

int execute_service(unsigned int key, int* client, char* args) {
    if (key > (BASE_SERVICE_LENGTH*counter_manager)) {
        return service_not_found;
    }

    if (services[key].fn == NULL) {
        return service_not_found;
    }

    services[key].fn(client, args);
    return 0;
}