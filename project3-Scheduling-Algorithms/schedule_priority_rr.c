// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// schedule_priority_rr.c
// This file includes the implementation of priority with round-robin
// scheduling algorithm, which schedules tasks in order of priority 
// and uses round-robin scheduling for tasks with equal priority.

#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list;
int size = 0;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// add a task to the list in lexicograhical order (for grading puproses)
// allocate new memory
// initialize all the tasks' date fields with values
void add(char *name, int priority, int burst) {
  Task *temp = malloc(sizeof(Task));
  temp->name = name;
  temp->priority = priority;
  temp->burst = temp->remaining_burst = burst;

  if (size == 0 || temp->priority > task_list->task->priority) {
    insert(&task_list, temp);
  }

  else if (temp->priority == task_list->task->priority 
    && comesBefore(temp->name, task_list->task->name)) {
      insert(&task_list, temp);
  }

  else {
    struct node *curr = task_list->next;
    struct node *prev = task_list;
    while (curr != NULL && curr->task->priority >= temp->priority
        && comesBefore(curr->task->name, temp->name)) {
          prev = curr;
          curr = curr->next;
    }
    prev->next = insert(&curr, temp);
  }
  size++;
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
  struct node *tempPrev = task_list;
  struct node *tempCurr = task_list->next;
  struct node *temp = task_list;

  int switchCounter = 0;
  int maxP = 0; // max priority in list
  int sizeTemp = size;
  int time = 0;

  // check is there are tasks with the same priority
  // set hasEqual to 'true' if found
  while(sizeTemp > 0) {
    Task *task = tempCurr->task;
    if(task->priority == tempPrev->task->priority) {
      task->hasEqual = true;
      tempPrev->task->hasEqual = true;
    }
    if(tempCurr->next != NULL) {
      if(task->priority == tempCurr->next->task->priority) {
        task->hasEqual = true;
        tempCurr->next->task->hasEqual = true;
      }
    }
    else {
      break;
    }

    tempPrev = tempCurr;
    tempCurr = tempCurr->next;
    sizeTemp--;
  }

  // while list not empty
  while (size > 0) {
    Task *task = temp->task;
    maxP = task->priority;
    
    if(task->hasEqual == false) { // only one task with this priority
      switchCounter++;
      run(task, task->burst);
      time += task->burst;
      delete (&task_list, task); // delete the node from list
      temp = temp->next;
      size--;
    }

    else if (task->burst > QUANTUM) { // remaining burst greater than quantum
      switchCounter++;      
      run(task, QUANTUM); // run for quantum
      time += QUANTUM;
      task->burst -= QUANTUM; // update burst
      temp = temp->next;
    }

    else { // remaining bust smalet than quantum
      switchCounter++;
      run(task, task->burst);
      time += task->burst;
      delete(&task_list, task); // remove task
      temp = temp->next;
      size--;
    }

    // if end of list or current priority less than max priority
    if (temp == NULL || temp->task->priority < maxP) {
      temp = task_list; // go back to task_list
    }
    printf("\tTime is now: %d\n", time);
  }
  int dispatcherTime = time + switchCounter - 1;
  utilizationCPU(time, dispatcherTime);
}
