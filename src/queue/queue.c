#include "queue.h"

Queue *queueInit(int type, int priority, int quantum)
{
    Queue *queue = calloc(1, sizeof(Queue));
    queue->type = type;
    queue->priority = priority;
    queue->quantum = priority * quantum;

    return queue;
}