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
} Task;

#endif