// Incluimos standard I/O, standard library
#include <stdio.h>
#include <stdlib.h>

#pragma once

// // https://www.programiz.com/c-programming/c-enumeration
// enum DesignStatus
// {
//     RUNNING,
//     READY,
//     WAITING,
//     FINISHED
// };

struct process;
typedef struct process Process;

struct process
{
    /** Enunciado info minima */
    int pid;
    char name[32];

    // enum DesignStatus p_status;
    int status;  // READY (0), RUNNING (1), , WAITING(2) o FINISHED (3).
    int start_time;
    int cycles;
    int wait;
    int waiting_delay;
    int s_aging_time;
    int s_completed;     // Es 1 si llega a envejecimiento mientras esta ejecutando CPU
    int s_extra_counter; // cuenta los ciclos de envejecimiento extras acumulados si estaba en CPU
    int priority;        // CASOS : fifo_1= 0, fifo_2= 1, sjf= 2

    // ENUNCIADO -- output

    int cpu_number_choice;        // El numero de veces que el proceso fue elegido para usar alguna CPU
    int cpu_exec_counter;         // Numero de ciclos trabajados en CPU - (A-i)
    int cpu_actual;               // Numero de ciclos trabajados en CPU desde que entro a CPU
    int cpu_number_interruptions; // El numero de veces que fue interrumpido. Este equivale al numero de veces que el scheduler saco al proceso de la CPU

    int turnaround_time; // Turnaround time

    int response_time; // Response time

    int waiting_time;        // Waiting time (suma del tiempo en el que el proceso esta en estado READY y WAITING.)
    int cycles_left_counter; // Ciclos que le faltan para terminar
    int s_aging_counter;     // Ciclos de envejecimiento
    int wait_counter;        // Ciclos del wait actual

    // Order de procesos
    Process *next;
    Process *prev;
};

Process *processInit(char *NOMBRE_PROCESO, int PID, int TIEMPO_INICIO, int CYCLES, int WAIT, int WAITING_DELAY, int S);
void deleteProcess(Process *process);
void checkSStatus(Process *check);
