#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "key_manager.h"

key_item_t* key_head = NULL;
key_item_t* key_tail = NULL;

void create_key_bucket(const char* key) {
    if (strlen(key) != 37) {
        fprintf(stderr, "Key must be UUID\n");
    }
    mount_key(key);
}

int find_service_in_key(unsigned int pos_service, const key_item_t* key) {
    key_item_service_t* it;
    it = key->service_head;
    while(it != NULL) {
        if (it->service == pos_service) {
            return 0;
        }

        it = it->next;
    }

    return -1;
}

int attach_service_to_key(unsigned int pos_service, const char* key) {
    key_item_t* found = find_key(key);

    if (found == NULL) {
        return -1; //TODO: provide a proper error code
    }

    key_item_service_t* new_key_service = malloc(sizeof(key_item_service_t));
    new_key_service->service = pos_service;
    new_key_service->next = NULL;

    if (found->service_tail == NULL) {
        found->service_head = new_key_service;
    } else {
        found->service_tail->next = new_key_service;
    }

    found->service_tail = new_key_service;

    return 0;
}

key_item_t* find_key(const char* key) {
    key_item_t* it;
    it = key_head;
    while (it != NULL) {
        if (strcmp(it->key, key) == 0) {
            return it;
        }

        it = it->next;
    }

    return NULL;
}

void mount_key(const char* key) {
    key_item_t* new_key = malloc(sizeof(key_item_t));
    strcpy(new_key->key, key);
    new_key->service_head = NULL;
    new_key->service_tail = NULL;
    new_key->next = NULL;

    if (key_tail == NULL) {
        key_head = new_key;
    } else {
        key_tail->next = new_key;
    }

    key_tail = new_key;
}