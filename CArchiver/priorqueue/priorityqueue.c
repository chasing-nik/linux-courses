#include "priorityqueue.h"

static void PushUp(priorityQueue *pqueue, int index){
    huffman_node_t *tempnode;
    while(index>0 && pqueue->queue[index]->count<pqueue->queue[(index-1)/2]->count){
        tempnode=pqueue->queue[index];
        pqueue->queue[index]=pqueue->queue[(index-1)/2];
        pqueue->queue[(index-1)/2]=tempnode;
        index=(index-1)/2;
    }
}

void Insert(priorityQueue *pqueue, huffman_node_t *node){
    if(pqueue->size<CHARCOUNT){
        pqueue->queue[pqueue->size++]=node;
        PushUp(pqueue,pqueue->size-1);
    }
}

static void PushDown(priorityQueue *pqueue,int index){
    int leftChild,rightChild, minElem;
    huffman_node_t *tempnode;
    while(2*index+1<pqueue->size){
        leftChild=2*index+1;
        rightChild=2*index+2;
        minElem=leftChild;
        if(rightChild<pqueue->size && pqueue->queue[rightChild]->count<pqueue->queue[leftChild]->count)
            minElem=rightChild;
        if(pqueue->queue[index]->count<=pqueue->queue[minElem]->count)
            break;
        tempnode=pqueue->queue[index];
        pqueue->queue[index]=pqueue->queue[minElem];
        pqueue->queue[minElem]=tempnode;
        index=minElem;
    }
}

huffman_node_t *Extract(priorityQueue *pqueue){
    huffman_node_t *tempnode;
    if(pqueue->size>0){
        tempnode=pqueue->queue[0];
        pqueue->queue[0]=pqueue->queue[pqueue->size-1];
        pqueue->queue[pqueue->size-1]=tempnode;
        --pqueue->size;
        PushDown(pqueue,0);
        return pqueue->queue[pqueue->size];
    }
    return null;
}







