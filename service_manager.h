#ifndef HASH_SERVICE_MANAGER_H
#define HASH_SERVICE_MANAGER_H

#define BASE_SERVICE_LENGTH 4096

struct service_manager {
    unsigned int key;
    void* (*fn)(void*);
};
typedef struct service_manager service_manager_t;

void create_manager(void);
void clear_manager(void);
unsigned int create_service(void* (*)(void*));
int execute_service(unsigned int, void*);

#endif //HASH_SERVICE_MANAGER_H
