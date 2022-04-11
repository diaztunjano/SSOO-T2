#pragma once
#include "../process/process.h"

struct queue;
typedef struct queue Queue;
struct queue
{
    int priority; //2 para el de más prioridad y 1 para el de menos (FIFO)
    int type;     // 0 es FIFO y 1 es SJF
    int quantum;  // será == 0 en Queue SJF

    int length;
    Process *head;
    Process *tail;
};

Queue *queueInit(int type, int priority, int quantum);
void showQueue(Queue *queue);

void addProcessToQueue(Queue *queue, Process *new);
Process *getProcessFromQueue(Queue *queue, int pid);

void insertSortbyStartTime(Queue *queue, Process *node);
void insertSortbyCyclesLeft(Queue *queue, Process *node);
void addToFIFOQueue(Queue *queue, Process *node_to_add);


void eraseTail(Queue *queue);
void eraseHead(Queue *queue);