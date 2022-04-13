// Incluimos standard I/O, standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

Process *processInit(char *NOMBRE_PROCESO, int PID, int TIEMPO_INICIO, int CYCLES, int WAIT, int WAITING_DELAY, int S)
{
  Process *new_process = calloc(1, sizeof(Process));
  new_process->pid = PID;
  // https://stackoverflow.com/questions/6008733/expression-must-be-a-modifiable-l-value
  strcpy(new_process->name, NOMBRE_PROCESO);
  new_process->start_time = TIEMPO_INICIO;
  new_process->cycles = CYCLES;
  new_process->wait = WAIT;
  new_process->waiting_delay = WAITING_DELAY;
  new_process->s_aging_time = S;
  return new_process;
}

void deleteProcess(Process *new_process)
{
  free(new_process);
}

// Reviso si llega a envejecimiento al ejecutar
// o si necesito contar más ciclos despues de llegar a envejec.
void checkSStatus(Process *check)
{
  if (check->s_completed)
  {
    check->s_extra_counter++;
    printf("Contando en S extra counter\n");
  }
  if ((check->s_aging_counter) % (check->s_aging_time) == 0)
  {
    printf("Envejecimiento cumplido en CPU\n");
    check->s_completed = 1;
  }
}