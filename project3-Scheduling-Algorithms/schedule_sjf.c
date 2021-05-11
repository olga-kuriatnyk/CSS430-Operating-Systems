// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_sjf.c
// This file includes the implementation of shortest-job-first (SJF), 
// which schedules tasks in order of the length of the tasks' next CPU burst.

#include<string.h>
#include<stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list = NULL;

// add a task to the list
// allocate new memory
// initialize all the tasks' date fields with values
void add(char *name, int priority, int burst) {
  Task *task = malloc(sizeof(Task));
  task->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(task->name, name);
  task->priority = priority;
  task->burst = burst;
  insert(&task_list, task);
}

// pick the next task to execute with SJF
// 
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *shortest_job = task_list->task;
  struct node *list = task_list;
  while(list){
    if(list->task->burst <= shortest_job->burst){
      shortest_job = list->task;
    }
    list = list->next;
  }
  return shortest_job;
}

// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  int time = 0;
  while(task_list) {
    Task *task = pickNextTask();
    run(task, task->burst);
    time += task->burst;
    printf("\tTime is now: %d\n", time);
    delete(&task_list, task);
  }
}