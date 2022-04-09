#include "process.h"

Process *processInit(char NOMBRE_PROCESO, int PID, int TIEMPO_INICIO, int CYCLES, int WAIT, int WAITING_DELAY, int S)
{
    Process *new_process = calloc(1, sizeof(Process));
    new_process->pid = PID;
    new_process->start_time = TIEMPO_INICIO;
    new_process->cycles = CYCLES;
    new_process->wait = WAIT;
    new_process->waiting_delay = WAITING_DELAY;
    new_process->S_aging_time = S;
    return new_process
}
