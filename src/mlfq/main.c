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
		Process *new_process = processInit(*name, input_info_process[0], input_info_process[1], input_info_process[2], input_info_process[3], input_info_process[4], input_info_process[5]);

		// Lo meto a queue general sorted by time
		insertSortbyStartTime(start_time_queue, new_process);
		printf("\n");
	}
	showQueue(start_time_queue);

	int cycle_counter = 0;
	Process *current_process_in_cpu;

	// MAIN LOGIC:
	// Reviso para cada cola,
	while (fifo_1_queue->head || fifo_2_queue->head || sjf_queue->head || start_time_queue->head || current_process_in_cpu)
	{
		// Agrego a cola FIFO 1 desde cola Start_time que agrupa a todos sorted por start time
		while (cycle_counter <= start_time_queue->head->start_time)
		{
			printf("Entrando PID = %d | startTime = %d \n", start_time_queue->head->pid, start_time_queue->head->start_time);
			addProcessToQueue(fifo_1_queue, start_time_queue->head);
			eraseHead(start_time_queue);
		}

		cycle_counter++;
	}

	input_file_destroy(input_file);
}