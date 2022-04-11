#include "queue.h"

Queue *queueInit(int type, int priority, int quantum)
{
    Queue *queue = calloc(1, sizeof(Queue));
    queue->type = type;
    queue->priority = priority;
    queue->quantum = priority * quantum;

    return queue;
}

void insertSortbyStartTime(Queue *queue, Process *node)
{
    printf("Inserting Sorting by Start Time: %d\n", node->start_time);
    int node_to_compare = node->start_time;
    queue->length++; //agrego uno a su largo

    if (queue->head) //caso: comparar con nodes de queue
    {
        for (Process *node_in_queue = queue->head; node_in_queue; node_in_queue = node_in_queue->next)
        {
            if (node_to_compare < node_in_queue->start_time)
            {
                //Nodo nuevo es mejor que head actual
                //Cambio lugares con head
                if (node_in_queue->pid == queue->head->pid)
                {
                    queue->head = node;
                    node_in_queue->prev = node;
                    node->next = node_in_queue;
                    return;
                }
                else //Lo meto al medio entre node_in_queue y su prev
                {
                    node->next = node_in_queue;
                    Process *prev_original_node = node_in_queue->prev;
                    prev_original_node->next = node;
                    node->prev = prev_original_node;
                    node_in_queue->prev = node;
                    return;
                }
            }
        }
        // Lo meto a la cola, cambiando con el que esta en la cola actual
        Process *old_tail_node = queue->tail;
        queue->tail = node;
        old_tail_node->next = node;
        node->prev = old_tail_node;
    }
    else
    { //caso: no hay nadie, inserto el primero
        queue->head = node;
        queue->tail = node;
    }
}

void showQueue(Queue *queue)
{
    printf("Imprimiendo queue Largo = %d Headpid = %d \n", queue->length, queue->head->pid);

    for (Process *nodo = queue->head; nodo; nodo = nodo->next)
    {
        printf("PID = %d | Start Time = %d \n", nodo->pid, nodo->start_time);
    }
}