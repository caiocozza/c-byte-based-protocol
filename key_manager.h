#ifndef HASH_KEY_MANAGER_H
#define HASH_KEY_MANAGER_H

#include <uuid/uuid.h>

struct key_item_service {
    unsigned int service;
    struct key_item_service* next;
};
typedef struct key_item_service key_item_service_t;

struct key_item {
    char key[UUID_STR_LEN];
    key_item_service_t* service_head;
    key_item_service_t* service_tail;
    struct key_item* next;
};
typedef struct key_item key_item_t;

const char* create_key_bucket(void);
int find_service_in_key(unsigned int, const key_item_t*);
int attach_service_to_key(unsigned int, const char*);
key_item_t* find_key(const char*);
const char* create_key(void);

#endif //HASH_KEY_MANAGER_H
