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

// lists for info table
struct node *temp_list1, *temp_list2, *temp_list3, *temp_list4;
int size = 0;

// add a task to the list
// allocate new memory
// initialize all the tasks' date fields with values
void add(char *name, int priority, int burst) {
  Task *temp = malloc(sizeof(Task));
  temp->name = name;
  temp->priority = priority;
  temp->burst = burst;
  insert(&task_list, temp);
  size++;
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


// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  printf("\nFCFS: \n");

  int time = 0, switch_counter = 0;

  while(task_list) {
    Task *task = pickNextTask();
    run(task, task->burst);
    switch_counter++;
    
    // Table of information
    task->wt = time; // wait time
    task->rt = time; // response time
    time += task->burst;
    task->tat = time; // turnaround time    
    insert(&temp_list1, task);
    insert(&temp_list2, task);
    insert(&temp_list3, task);
    insert(&temp_list4, task);

    printf("\tTime is now: %d\n", time);
    delete (&task_list, task); // delete the node from list
  }
  // print CPU Utilization time
  int dispatcherTime = time + switch_counter - 1;
  utilizationCPU(time, dispatcherTime);

  // print the table of info
  tableOfInfo(size, temp_list1, temp_list2, temp_list3, temp_list4);
}

