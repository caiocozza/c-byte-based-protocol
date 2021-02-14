#ifndef SOCKET_QUEUE_H
#define SOCKET_QUEUE_H

struct node {
    struct node* next;
    int* item;
};
typedef struct node node_t;

void enqueue(int*);
int* dequeue(void);

#endif //SOCKET_QUEUE_H