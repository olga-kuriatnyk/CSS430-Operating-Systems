// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// task.h
// Representation of a task in the system.
// This class was modified

#include <stdbool.h>

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int remaining_burst;
    int ran_number;
    bool hasEqual;

    // for table of information
    int tat; // Turn Around time // Time of process submission to time of completion.
    int wt; // Wait Time // Total amount of time a process spends waiting in the ready queue.
    int rt; // Response Time // Time a process takes from submission to start of response.
} Task;

#endif