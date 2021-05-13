// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_priority.c
// This file includes the implementation of priority scheduling algorithm,
// which schedules tasks based on priority.

#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list;

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

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// pick the next task to execute with the highest priority
// for task with the same priority chose in lexicographical order
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *highest_priority_job = task_list->task;
  struct node *temp = task_list;
  while(temp){
    if(temp->task->priority > highest_priority_job->priority){
      highest_priority_job = temp->task;
    }
    else if (temp->task->priority == highest_priority_job->priority) {
      if (comesBefore(temp->task->name, highest_priority_job->name)) {
        highest_priority_job = temp->task;
      }
    }
    temp = temp->next;
  }
  return highest_priority_job;
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
    delete(&task_list, task);
  }
  int dispatcherTime = time + switch_counter - 1;
  utilizationCPU(time, dispatcherTime);
}