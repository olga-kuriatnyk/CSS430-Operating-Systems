#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <stdbool.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list = NULL;

// add a task to the list
void add(char *name, int priority, int burst) {
  Task *t = malloc(sizeof(Task));
  // allocate memory and then copy the name
  t->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(t->name, name);
  // priority and burst
  t->priority = priority;
  t->burst = burst;
  // insert into task list
  insert(&task_list, t);
}

// pick the next task to execute with FCFS
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  struct node *lastNode = task_list;
   while(lastNode->next) {
    lastNode = lastNode->next;
  }
  return lastNode->task;
}

// // PISAN CODE
// bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// // based on traverse from list.c
// // finds the task whose name comes first in dictionary
// Task *pickNextTask() {
//   // if list is empty, nothing to do
//   if (!task_list) {
//       return NULL;
//   }

//   struct node *temp;
//   temp = task_list;
//   Task *best_sofar = temp->task;

//   while (temp != NULL) {
//     if (comesBefore(temp->task->name, best_sofar->name)) {
//         best_sofar = temp->task;
//     }
//     temp = temp->next;
//    }
//   // delete the node from list, Task will get deleted later
//   delete (&task_list, best_sofar);
//   return best_sofar;
// }

// invoke the scheduler
void schedule() {
  int time = 0;
  while(task_list) {
    Task *t = pickNextTask();
    run(t, t->burst);
    time += t->burst;
    printf("\tTime is now: %d\n", time);
    delete(&task_list, t);
  }
}