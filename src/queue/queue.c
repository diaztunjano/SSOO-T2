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

void removeProcessFromQueue(Queue *queue, Process *process)
{
    if (process->prev && process->next)
    {
        process->next->prev = process->prev;
        process->prev->next = process->next;
    }
    else if (process->next)
    {
        // es head
        queue->head = process->next;
        queue->head->prev = NULL;
    }
    else if (process->prev)
    {
        // es la cola
        queue->tail = process->prev;
        queue->tail->next = NULL;
    }
    else
    {
        // es el unico proceso
        queue->head = NULL;
        queue->tail = NULL;
    }
    process->next = NULL;
    process->prev = NULL;
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

void eraseTail(Queue *queue)
{
    if (queue->head == queue->tail)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        Process *prev_tail = queue->tail;
        Process *new_tail = prev_tail->prev;

        queue->tail = new_tail;
        queue->tail->next = NULL;
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

Process *getProcessFromQueue(Queue *queue, int pid)
{
    for (Process *node_in_queue = queue->head; node_in_queue->next; node_in_queue = node_in_queue->next)
    {
        if (node_in_queue->pid == pid)
        {
            return node_in_queue;
        }
    }
    return NULL;
}

// Checkeo para colas FIFO 1 y FIFO 2
int excedesQuantum(Process *process, int quantum)
{
    if (process->priority == 0)
    {
        // Es por 2 por el caso de queue de mayor prioridad FIFO_1
        if (process->cpu_actual >= 2 * quantum)
        {
            return 1;
        }
    }
    else if (process->priority == 1)
    {
        if (process->cpu_actual >= quantum)
        {
            return 1;
        }
    }
    return 0;
}

void updateProcesses(Queue *queue, Queue *fifo1)
{
    for (Process *process = queue->head; process; process = process->next)
    {
        process->s_aging_counter++;
        if (process->p_status == WAITING)
        {
            process->wait_counter++;
        }
        if (process->wait_counter > process->waiting_delay)
        {
            process->p_status = READY;
            process->wait_counter = 0;
        }
        if (queue->priority != 0 && (process->s_aging_counter) % (process->s_aging_time) == 0)
        {
            Process *newPriority = process;
            removeProcess(queue, process);
            addProcessToQueue(fifo1, newPriority);
            process->s_aging_counter = 0;
            printf("[Salida a FIFO 1] Por envejecimiento\n");
        }
    }
}