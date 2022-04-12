// Incluimos standard I/O, standard library
#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process *processInit(char NOMBRE_PROCESO, int PID, int TIEMPO_INICIO, int CYCLES, int WAIT, int WAITING_DELAY, int S, enum DesignStatus P_STATUS, int PRIORITY)
{
    Process *new_process = calloc(1, sizeof(Process));
    new_process->pid = PID;
    new_process->start_time = TIEMPO_INICIO;
    new_process->cycles = CYCLES;
    new_process->wait = WAIT;
    new_process->waiting_delay = WAITING_DELAY;
    new_process->s_aging_time = S;
    new_process->p_status = P_STATUS;
    new_process->priority = PRIORITY;
    return new_process;
}


void delete_process(Process* new_process)
{
  free(new_process);
}
