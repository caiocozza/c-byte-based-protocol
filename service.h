#ifndef HASH_SERVICE_H
#define HASH_SERVICE_H

#define BASE_SERVICE_LENGTH 4096

struct service {
    unsigned int key;
    char service_name[64];
    void* (*fn)(int*, void*);
};
typedef struct service service_t;

static service_t* services;

void create_manager(void);
void clear_manager(void);
int service_exists(unsigned int);
unsigned int create_service(char* , void* (*fn)(int*,void*));
int execute_service(unsigned int, int*, char*);
int send_response(const int*, const char*);

#endif //HASH_SERVICE_H
