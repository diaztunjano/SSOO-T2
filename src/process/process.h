// Incluimos standard I/O, standard library
#include <stdio.h>
#include <stdlib.h>

#pragma once

// https://www.programiz.com/c-programming/c-enumeration
enum DesignStatus {
    RUNNING,
    READY,
    WAITING,
    FINISHED
};

struct process;
typedef struct process Process;

struct process
{
    /** Enunciado info minima */
    int pid;
    char *name[32];
    enum DesignStatus p_status;

    int start_time;
    int cycles;
    int wait;
    int waiting_delay;
    int s_aging_time;
    int priority;

    // ENUNCIADO -- output

    // El numero de veces que el proceso fue elegido para usar alguna CPU
    int cpu_number_choice;
    // El numero de veces que fue interrumpido. Este equivale al n ´ umero de veces que el ´ scheduler saco al proceso de la CPU
    int cpu_number_interruptions;
    // Turnaround time
    int turnaround_time;
    // Response time
    int response_time;
    // Waiting time (suma del tiempo en el que el proceso esta en estado READY y WAITING.)
    int waiting_time;
    // Numero de ciclos en CPU
    int cpu_exec_counter;
    // Ciclos que le faltan para terminar
    int cycles_left_counter;
    // Ciclos de envejecimiento
    int s_aging_counter;
    // Ciclos del wait actual
    int wait_counter;

    // Order de procesos
    Process *next;
    Process *prev;
};

Process *processInit(char NOMBRE_PROCESO, int PID, int TIEMPO_INICIO, int CYCLES, int WAIT, int WAITING_DELAY, int S, enum DesignStatus p_status, int PRIORITY);
void delete_process(Process* process);
