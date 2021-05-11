// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_rr.c
// This file includes the implementation of round-robin (RR) scheduling algorithm,
// where each task is run for a time quantum (or for the remainder of its CPU burst).

#include<string.h>
#include<stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list = NULL;
struct node *next_node;

// add a task to the list
// allocate new memory
// initialize all the tasks' date fields with values
void add(char *name, int priority, int burst) {
  Task *task = malloc(sizeof(Task));
  task->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(task->name, name);
  task->priority = priority;
  task->burst = task->remaining_burst = burst;
  insert(&task_list, task);
}

// pick the next task to execute with Round Robin
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *task = next_node->task;
  next_node = (next_node -> next) ? next_node->next : task_list;
  return task;
}

// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  int time = 0;
  next_node = task_list;
  while(task_list) {
    Task *task = pickNextTask();
    int slice = QUANTUM < task->remaining_burst ? QUANTUM : task->remaining_burst;
    run(task, slice);
    task->remaining_burst -= slice;
    time += slice;
    printf("\tTime is now: %d\n", time);
    if(!task->remaining_burst) {
      delete(&task_list, task);
    }
  }
}