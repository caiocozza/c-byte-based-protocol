#ifndef HASH_KEY_MANAGER_H
#define HASH_KEY_MANAGER_H

struct key_item_service {
    unsigned int service;
    struct key_item_service* next;
};
typedef struct key_item_service key_item_service_t;

struct key_item {
    char key[37];
    key_item_service_t* service_head;
    key_item_service_t* service_tail;
    struct key_item* next;
};
typedef struct key_item key_item_t;

void create_key_bucket(const char*);
int find_service_in_key(unsigned int, const key_item_t*);
int attach_service_to_key(unsigned int, const char*);
key_item_t* find_key(const char*);
void mount_key(const char*);

#endif //HASH_KEY_MANAGER_H
