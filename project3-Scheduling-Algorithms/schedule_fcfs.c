// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_fcfs.c
// This file includes the implementation of first-come, first-served (FCFS) scheduling
// algorithm, which schedules tasks in the order in which they request the CPU.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// add a task to the list
// allocate new memory
// initialize all the tasks' date fields with values
void add(char *name, int priority, int burst) {
  Task *temp = malloc(sizeof(Task));
  temp->name = name;
  temp->priority = priority;
  temp->burst = burst;
  insert(&task_list, temp);
}

// finds the task whose name comes first in dictionary for grading puproses
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  struct node *temp;
  temp = task_list;
  Task *best_sofar = temp->task;
  while (temp) {
    if (comesBefore(temp->task->name, best_sofar->name)) {
      best_sofar = temp->task;
    }
    temp = temp->next;
   }
  return best_sofar;
}

// calculete CPU utilization
float utilizationCPU(int totalTime, int dispatcherTime) {
  float result = (totalTime / (float)dispatcherTime) * 100;
  printf("CPU Utilization: %.2f%%\n", result);
  return result;
}

// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  int time = 0;
  int switch_counter = 0;
  while(task_list) {
    Task *task = pickNextTask();
    run(task, task->burst);
    switch_counter++;
    time += task->burst;
    printf("\tTime is now: %d\n", time);
    delete (&task_list, task); // delete the node from list
  }
  int dispatcherTime = time + switch_counter - 1;
  utilizationCPU(time, dispatcherTime);
}