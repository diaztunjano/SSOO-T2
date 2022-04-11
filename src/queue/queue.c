#include "queue.h"

Queue *queueInit(int type, int priority, int quantum)
{
    Queue *queue = calloc(1, sizeof(Queue));
    queue->type = type;
    queue->priority = priority;
    queue->quantum = priority * quantum;

    return queue;
}

void addProcessToQueue(Queue *queue, Process *node_to_add)
{
    printf("(%d)\n", node_to_add->pid);
    //Caso SJF inserto con type = 1
    if (queue->type == 1)
    {
        printf("CASO SJF\n");
        insertSortbyCyclesLeft(queue, node_to_add);
    }
    //Caso FIFO agrego al final
    else
    {
        printf("CASO FIFO\n");
        addToFIFOQueue(queue, node_to_add);
    }
    showQueue(queue);
    queue->length++;
}

void eraseHead(Queue *queue)
{
    printf("ERASING HEAD Queue: %d (0 es FIFO y 1 es SJF)\n", queue->type);

    if (queue->head->next)
    {
        Process *new_head = queue->head->next;
        queue->head = new_head;
        new_head->prev = NULL;
    }
    else
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
}

void addToFIFOQueue(Queue *queue, Process *node_to_add)
{
    if (queue->length == 0)
    {
        queue->head = node_to_add;
        printf("HEAD con PID: %d\n", node_to_add->pid);
    }
    else
    {
        printf("TAIL con PID: %d\n", node_to_add->pid);
        queue->tail->next = node_to_add;
        node_to_add->prev = queue->tail;
    }
    //printf("agregado el estrecho con id = %i al padre %i\n", person -> id, person ->parent ->id);
    queue->tail = node_to_add;
}

void insertSortbyStartTime(Queue *queue, Process *node)
{
    printf("Inserting Sorting by Start Time: %d\n", node->start_time);
    int node_to_compare = node->start_time;
    queue->length++; //agrego uno a su largo

    if (queue->head) //caso: comparar con nodes de queue
    {
        // Itero uno por uno, cambiando al next
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

    for (Process *node_in_queue = queue->head; node_in_queue; node_in_queue = node_in_queue->next)
    {
        printf("PID = %d | Start Time = %d \n", node_in_queue->pid, node_in_queue->start_time);
    }
}

// Similar ejecucion al insertSortbyStartTime
void insertSortbyCyclesLeft(Queue *queue, Process *node_to_add)
{
    printf("Entering CyclesLeft %d\n", node_to_add->cycles_left_counter);
    int node_to_compare = (node_to_add->cycles) - (node_to_add->cpu_exec_counter);
    queue->length++;
    if (queue->head)
    {
        for (Process *node_in_queue = queue->head; node_in_queue; node_in_queue = node_in_queue->next)
        {
            if (node_to_compare < node_in_queue->start_time)
            {
                //Mejor que Head
                if (node_in_queue->pid == queue->head->pid)
                {
                    queue->head = node_to_add;
                    node_in_queue->prev = node_to_add;
                    node_to_add->next = node_in_queue;
                    return;
                }
                else
                {
                    node_to_add->next = node_in_queue;
                    Process *prev_original_node = node_in_queue->prev;
                    prev_original_node->next = node_to_add;
                    node_to_add->prev = prev_original_node;
                    node_in_queue->prev = node_to_add;
                    return;
                }
            }
        }
        Process *old_tail_node = queue->tail;
        queue->tail = node_to_add;
        old_tail_node->next = node_to_add;
        node_to_add->prev = old_tail_node;
    }
    else
    {
        queue->head = node_to_add;
        queue->tail = node_to_add;
    }
}
