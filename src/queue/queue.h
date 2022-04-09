// Incluimos standard I/O, standard library
#include <stdio.h>
#include <stdlib.h>

#pragma once

struct queue;
typedef struct queue Queue;

struct queue
{
    /** El valor que guarda el pixel */
    int pid;
    char name[32];
    int priority;
    enum status
    {
        RUNNING,
        READY,
        WAITING,
        FINISHED
    };

    // Times
    int remaining_time;
    int time_a_cpu_burst;
    int time_b_io_burst;
};