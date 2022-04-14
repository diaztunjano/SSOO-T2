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
	char *output_name = (char *)argv[2];
	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("----------------------------\n");
	printf("Procesos:\n");

	// Quantum de input
	int quantum = atoi(argv[3]);
	printf("Quantum = %d\n", quantum);

	// Creo cada cola independiente
	Queue *start_time_queue = queueInit(0, 0, 0); //(type, priority, quantum)
	Queue *finished_queue = queueInit(0, 0, 0);	  // aca guardo todos para despues free
	Queue *fifo_1_queue = queueInit(0, 0, quantum * 2);
	Queue *fifo_2_queue = queueInit(0, 1, quantum);
	Queue *sjf_queue = queueInit(1, 2, 0);

	// Info para el proceso
	int input_info_process[6];
	char name[10] = "";

	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("(%d)\n", j);
			if (j) //Saco atributos de ese proceso
			{
				input_info_process[j - 1] = atoi(input_file->lines[i][j]);
				printf("%s ", input_file->lines[i][j]);
				printf("info_input: %d | ", input_info_process[j - 1]);
			}
			else //Saco nombre del proceso
			{
				strcpy(name, input_file->lines[i][j]);
				printf("NAME = %s\n", name);
			}
		}

		// Creo proceso con info anterior
		Process *new_process = processInit(name, input_info_process[0], input_info_process[1], input_info_process[2], input_info_process[3], input_info_process[4], input_info_process[5]);

		// Lo meto a queue general sorted by time
		insertSortbyStartTime(start_time_queue, new_process);
	}

	int cycle_counter = 0;
	Process *current_process_in_cpu = NULL;

	// MAIN LOGIC:
	// Reviso para cada cola si es que aun tiene procesos
	while (fifo_1_queue->head || fifo_2_queue->head || sjf_queue->head || start_time_queue->head || current_process_in_cpu != NULL)
	{
		printf("---------------------------\n");
		printf("Ciclo: %d\n", cycle_counter);

		// Reviso proceso actual en CPU
		if (current_process_in_cpu != NULL)
		{
			/* Update de contadores */
			current_process_in_cpu->cpu_exec_counter++;
			current_process_in_cpu->s_aging_counter++;
			current_process_in_cpu->cpu_actual++;
			checkSStatus(current_process_in_cpu);
			// Aca ya supero su tiempo de procesamiento
			// Calculo turnaround time y waiting time
			//Lo agrego a los procesos terminados (para poder después hacerle free a todos)
			if (current_process_in_cpu->cpu_exec_counter >= current_process_in_cpu->cycles)
			{
				printf("Proceso %d termino \n", current_process_in_cpu->pid);
				current_process_in_cpu->turnaround_time = (cycle_counter - current_process_in_cpu->start_time);
				current_process_in_cpu->waiting_time = (current_process_in_cpu->turnaround_time) - (current_process_in_cpu->cpu_exec_counter);
				addProcessToQueue(finished_queue, current_process_in_cpu);
				current_process_in_cpu = NULL;
			}

			// Proceso cede CPU y ahora espera input. Lo cambio de cola
			// Le cambio status a waiting, inicio el wait counter y +1 a interrupciones
			else if (current_process_in_cpu->cpu_exec_counter % current_process_in_cpu->wait == 0)
			{
				printf("Proceso %d cede CPU \n", current_process_in_cpu->pid);
				current_process_in_cpu->status = 2;
				current_process_in_cpu->wait_counter = 0;

				// Checkeo prioridad para agregarlo a FIFO_1 o FIFO_2
				if (current_process_in_cpu->priority == 0 || current_process_in_cpu->priority == 1)
				{
					// Agrego a FIFO_1
					addProcessToQueue(fifo_1_queue, current_process_in_cpu);
				}
				else
				{
					// Esto indica que ya llega a envejecimiento mientras esta ejecutando CPU
					if (current_process_in_cpu->s_completed)
					{
						// Agrego el proceso a FIFO_1
						// Actualizo s_completed = 0, s_aging_counter (ciclos envejec.), y reseteo s_extra_counter = 0
						addProcessToQueue(fifo_1_queue, current_process_in_cpu);
						current_process_in_cpu->s_completed = 0;
						current_process_in_cpu->s_aging_counter = current_process_in_cpu->s_extra_counter;
						current_process_in_cpu->s_extra_counter = 0;
						printf("[WAIT Salida] S Completed durante CPU\n");
					}
					else
					{
						// Agrego el proceso a FIFO 2
						addProcessToQueue(fifo_2_queue, current_process_in_cpu);
					}
				}
				// NULL pq ahora paso a waiting
				current_process_in_cpu = NULL;
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
					// Cambio a FIFO 2
					////// Falta crear la funcion tipo addProcess(Queue, process) para usar aca
					addProcessToQueue(fifo_2_queue, current_process_in_cpu);
				}
				else
				{
					// Caso que cumple envejecimiento durante ejecucion
					if (current_process_in_cpu->s_completed)
					{

						// Añado a FIFO_1
						// Reseteo s_completed = 0
						// Update a s_aging_counter = s_extra_counter
						// s_extra_counter = 0
						addProcessToQueue(fifo_1_queue, current_process_in_cpu);
						current_process_in_cpu->s_completed = 0;
						current_process_in_cpu->s_aging_counter = current_process_in_cpu->s_extra_counter;
						current_process_in_cpu->s_extra_counter = 0;
						printf("[QUANTUM Salida] S Completed durante CPU\n");
					}
					else
					{
						/* Ultimo caso: Añado a SJF Queue */
						addProcessToQueue(sjf_queue, current_process_in_cpu);
					}
					// NULL pq lo estoy cambiando de cola
				}
				current_process_in_cpu = NULL;
			}
		}

		//////////////////////////////////////////////// Por implementar:
		/// Revisar Head de start_time_queue  y si su tiempo de inicio es menor a cycle_counter, entra a FIFO_1

		/// Updatear procesos en todas las queues.
		/// Updateo contadores de status, wait_counter, o bien entrarlo a fifo_1 si es el caso
		/// podria ser una funcion de la misma queue

		while (start_time_queue->head && start_time_queue->head->start_time <= cycle_counter)
		{
			Process *enteringProcess = start_time_queue->head;
			eraseHead(start_time_queue);
			addProcessToQueue(fifo_1_queue, enteringProcess);
		}

		updateProcesses(fifo_1_queue, fifo_1_queue);
		updateProcesses(fifo_2_queue, fifo_1_queue);
		updateProcesses(sjf_queue, fifo_1_queue);

		/////////////////////////////
		/// CASO si es que no hay un current_process_in_CPU
		if (!current_process_in_cpu)
		{
			// No hay proceso, tengo que buscar cual esta ready para execution
			////// POR implementar: funcion que retorne ese proceso (Puede ser dentro de queue.c)
			printf("No hay proceso actual en CPU\n");
			current_process_in_cpu = processReadyForExecution(fifo_1_queue);
			if (current_process_in_cpu == NULL)
			{
				current_process_in_cpu = processReadyForExecution(fifo_2_queue);
			}
			if (current_process_in_cpu == NULL)
			{
				current_process_in_cpu = processReadyForExecution(sjf_queue);
			}
			if (current_process_in_cpu == NULL)
			{
				printf("No hay proceso para ejecutar\n");
			}
			if (current_process_in_cpu != NULL)
			{
				if (!current_process_in_cpu->cpu_number_choice)
				{
					current_process_in_cpu->response_time = (cycle_counter) - (current_process_in_cpu->start_time);
				}
				current_process_in_cpu->cpu_number_choice++;
				current_process_in_cpu->cpu_actual = 0;
				printf("Entra proceso desde lista %d\n", current_process_in_cpu->priority);
			}
		}

		cycle_counter++;
	}

	///////////////////// PROCESO DE ESCRITURA DE RESULTADOS:

	FILE *fptr;

	// opening file in writing mode
	fptr = fopen(output_name, "w");

	// exiting program
	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
	int counter = 0;
	printf("WRITING OUTPUT FILE\n");
	for (Process *process = finished_queue->head; process; process = process->next)
	{
		printf("(%d)\n", counter);
		counter++;
		fprintf(fptr, "%s,%d,%d,%d,%d,%d\n", process->name, process->cpu_number_choice,
				process->cpu_number_interruptions, process->turnaround_time, process->response_time, process->waiting_time);
		printf("%s, %d, %d, %d , %d, %d\n", process->name, process->cpu_number_choice,
			   process->cpu_number_interruptions, process->turnaround_time, process->response_time, process->waiting_time);
	}
	fclose(fptr);

	///////////////////// LIBERO MEMORIA de Finished processes y listas

	if (finished_queue->head)
	{
		Process *curr = finished_queue->head->next;
		Process *prev = finished_queue->head;

		while (curr)
		{
			deleteProcess(prev);
			prev = curr;
			curr = curr->next;
		}
		deleteProcess(prev);
	}
	free(finished_queue);
	free(fifo_1_queue);
	free(fifo_2_queue);
	free(start_time_queue);
	free(sjf_queue);

	input_file_destroy(input_file);
}