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
  temp->burst = temp->remaining_burst = burst;
  insert(&task_list, temp);
  size++;
}

// pick the next task to execute with Round Robin
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }

  struct node *temp;
  temp = task_list;
  Task *best_sofar = temp->task;
  while (temp) {
    if (temp->task->ran_number < best_sofar->ran_number) {
      best_sofar = temp->task;
    }
    else if (temp->task->ran_number == best_sofar->ran_number) {
      if (comesBefore(temp->task->name, best_sofar->name)) {
        best_sofar = temp->task;
      }
    }

    temp = temp->next;
   }
  return best_sofar;
}

// invoke the scheduler
// print total time used by CPU after finishing each task 
void schedule() {
  printf("\nRR: \n");

  int time = 0, switchCounter = 0;

  while(task_list) {
    Task *task = pickNextTask();
    task->ran_number += 1;
    switchCounter++;

    if (task->remaining_burst > QUANTUM) {
      run(task, QUANTUM);
      if (task->rt == 0) {
        printf("Time: %d\n", time);
        task->rt = time; // response time
      }
      time += QUANTUM;
      task->remaining_burst -= QUANTUM; // update burst
    }

    else {
      run(task, task->remaining_burst);
      
      // Table of information
      if (task->burst == task->remaining_burst) {
        task->rt = time; // response time
      }
      time += task->remaining_burst;
      task->wt = time - task->burst; // wait time
      task->tat = time; // turnaround time
      insert(&temp_list1, task);
      insert(&temp_list2, task);
      insert(&temp_list3, task);
      insert(&temp_list4, task);

      task->remaining_burst = 0;
      delete(&task_list, task);
    }
    printf("\tTime is now: %d\n", time);
  }
  // print CPU Utilization time
  int dispatcherTime = time + switchCounter -1;
  utilizationCPU(time, dispatcherTime);

  // print the table of info
  tableOfInfo(size, temp_list1, temp_list2, temp_list3, temp_list4);
}