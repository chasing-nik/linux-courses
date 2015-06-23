#ifndef PRIORITYQUEUE_H_INCLUDED
#define PRIORITYQUEUE_H_INCLUDED
#include "huffman.h"

typedef struct{
    huffman_node_t *queue[CHARCOUNT];
    int size;
}priorityQueue;

void Insert(priorityQueue *pqueue, huffman_node_t *node);
huffman_node_t *Extract(priorityQueue *pqueue);


#endif // PRIORITYQUEUE_H_INCLUDED
