#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node * next;
} NODE;

typedef struct Queue {
    NODE * front;
    NODE * rear;
} QUEUE;

void initQueue();

void isEmpty();

int main(){


    return 0;
}