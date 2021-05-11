// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_priority.c
// This file includes the implementation of priority scheduling alforithm,
// which schedules tasks based on priority.

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

// pick the next task to execute with the highest priority
// for task with the same priority chose in lexicographical order
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *highest_priority_job = task_list->task;
  struct node *temp = task_list;
  while(temp){
    if(temp->task->priority >= highest_priority_job->priority){
      highest_priority_job = temp->task;
    }
    temp = temp->next;
  }
  return highest_priority_job;
}

// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  while(task_list) {
    int time = 0;
    Task *task = pickNextTask();
    run(task, task->burst);
    time += task->burst;
    printf("\tTime is now: %d\n", time);
    delete(&task_list, task);
  }
}