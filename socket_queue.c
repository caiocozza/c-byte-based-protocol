#include <stdlib.h>
#include "socket_queue.h"

node_t* head = NULL;
node_t* tail = NULL;

void enqueue(int* item) {
    node_t* new_node = malloc(sizeof(node_t));
    new_node->item = item;
    new_node->next = NULL;

    if (tail == NULL) {
        head = new_node;
    } else {
        tail->next = new_node;
    }
    
    tail = new_node;
}

int* dequeue() {
    if (head == NULL) {
        return NULL;
    } else {
        int* item = head->item;
        node_t *temp = head;
        head = head->next;

        if (head == NULL) {
            tail = NULL;
        }

        free(temp);
        return item;
    }
}