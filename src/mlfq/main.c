#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> // strcpy
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("----------------------------\n");
	printf("Procesos:\n");

	// Quantum de input
	int quantum = atoi(argv[2]);
	printf("Quantum = %d\n", quantum);

	// Creo cada cola independiente
	Queue *start_time_queue = queueInit(0, 0, quantum); //(type, priority, quantum)
	Queue *finished_queue = queueInit(0, 0, quantum);	// aca guardo todos para despues free
	Queue *fifo_1_queue = queueInit(0, 2, quantum);
	Queue *fifo_2_queue = queueInit(0, 1, quantum);
	Queue *sjf_queue = queueInit(1, 0, quantum);

	// Info para el proceso
	int input_info_process[6];
	char name[10];

	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("(%d)\n", j);
			if (j) //Saco atributos de ese proceso
			{
				input_info_process[j - 1] = atoi(input_file->lines[i][j]);
				printf("%s ", input_file->lines[i][j]);
				printf("attrb: %d | ", input_info_process[j - 1]);
			}
			else //Saco nombre del proceso
			{
				strcpy(name, input_file->lines[i][j]);
				printf("nombre = %s", name);
			}
		}

		// Creo proceso con info anterior
		Process *new_process = processInit(*name, input_info_process[0], input_info_process[1], input_info_process[2], input_info_process[3], input_info_process[4], input_info_process[5], READY, 2); //asp generales: entran a p_i=2 en estado READY

		// Lo meto a queue general sorted by time
		insertSortbyStartTime(start_time_queue, new_process);
		printf("\n");
	}
	showQueue(start_time_queue);

	int cycle_counter = 0;
	Process *current_process_in_cpu = NULL;

	// MAIN LOGIC:
	// Reviso para cada cola,
	while (fifo_1_queue->head || fifo_2_queue->head || sjf_queue->head || start_time_queue->head || current_process_in_cpu != NULL)
	{
		printf("\nCiclo: %d\n", cycle_counter);

		// Reviso proceso actual en CPU
		if (current_process_in_cpu != NULL)
		{
			/* Update de contadores */
			current_process_in_cpu->cpu_exec_counter++;
			current_process_in_cpu->s_aging_counter++;
			current_process_in_cpu->cpu_actual++;

			// Aca ya supero su tiempo de procesamiento
			// Calculo turnaround time y waiting time
			//Lo agrego a los procesos terminados (para poder después hacerle free a todos)
			if (current_process_in_cpu->cpu_exec_counter >= current_process_in_cpu->cycles)
			{
				printf("Proceso %d termino \n", current_process_in_cpu->pid);
			}

			// Proceso cede CPU y ahora espera input. Lo cambio de cola
			// Le cambio status a waiting, inicio el wait counter y +1 a interrupciones
			else if (current_process_in_cpu->cpu_exec_counter % current_process_in_cpu->wait == 0)
			{
				/* code */
				printf("Proceso %d cede CPU \n", current_process_in_cpu->pid);

				// Checkeo prioridad para agregarlo a FIFO_1 o FIFO_2
				if (current_process_in_cpu->priority == 0 || current_process_in_cpu == 1)
				{
					/* code */
					// Agrego a FIFO_1
					////// Falta crear la funcion tipo addProcess(Queue, process) para usar aca
				}
				else
				{
					// Esto indica que ya llega a envejecimiento mientras esta ejecutando CPU
					if (current_process_in_cpu->s_completed)
					{
						/* code */
						// Agrego el proceso a FIFO_1
						// Actualizo s_completed = 0, s_aging_counter (ciclos envejec.), y reseteo s_extra_counter = 0
					}
					else
					{
						// Agrego el proceso a FIFO 2
					}
				}
				// NULL pq ahora paso a waiting
				current_process_in_cpu == NULL;
			}

			// Cuando un proceso usa todo su quantum en una determinada cola, se reduce su prioridad pasando a la cola siguiente
			else if (excedesQuantum(current_process_in_cpu, quantum) == 1)
			{
				/* code */
				printf("Proceso %d excede su quantum \n", current_process_in_cpu->pid);
				current_process_in_cpu->cpu_number_interruptions++;

				// Lo cambio de cola, si == 0, estaba en FIFO_1, lo paso a FIFO_2
				if (current_process_in_cpu->priority == 0)
				{
					/* code */
					// Cambio a FIFO 2
					////// Falta crear la funcion tipo addProcess(Queue, process) para usar aca
				}
				else
				{
					// Caso que cumple envejecimiento durante ejecucion
					if (current_process_in_cpu->s_completed)
					{
						/* code */
						// Añado a FIFO_1
						// Reseteo s_completed = 0
						// Update a s_aging_counter = s_extra_counter
						// s_extra_counter = 0
					}
					else
					{
						/* Ultimo caso: Añado a SJF Queue */
					}
					// NULL pq lo estoy cambiando de cola
					current_process_in_cpu = NULL;
				}
			}
		}

		//////////////////////////////////////////////// Por implementar:
		/// Revisar Head de start_time_queue  y si su tiempo de inicio es menor a cycle_counter, entra a FIFO_1

		/// Updatear procesos en todas las queues.
		/// Updateo contadores de status, wait_counter, o bien entrarlo a fifo_1 si es el caso
		/// podria ser una funcion de la misma queue

		/////////////////////////////
		/// CASO si es que no hay un current_process_in_CPU
		if (!current_process_in_cpu)
		{
			/* code */
			// No hay proceso, tengo que buscar cual esta ready para execution
			////// POR implementar: funcion que retorne ese proceso (Puede ser dentro de queue.c)
		}

		cycle_counter++;
	}

	///////////////////// PROCESO DE ESCRITURA DE RESULTADOS:

	///////////////////// LIBERO MEMORIA de Finished processes y listas

	input_file_destroy(input_file);
}