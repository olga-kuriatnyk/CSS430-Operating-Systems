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
void add(char *name, int priority, int burst) {
  Task *temp = malloc(sizeof(Task));
  // allocate memory and then copy the name
  temp->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(temp->name, name);
  // priority and burst
  temp->priority = priority;
  temp->burst = temp->remaining_burst = burst;
  // insert into task list
  insert(&task_list, temp);
}

// pick the next task to execute with Round Robin
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *ret = next_node->task;
  next_node = (next_node -> next) ? next_node->next : task_list;
  return ret;
}

// invoke the scheduler
void schedule() {
  int time = 0;
  next_node = task_list;
  while(task_list) {
    Task *temp = pickNextTask();
    int slice = QUANTUM < temp->remaining_burst ? QUANTUM : temp->remaining_burst;
    run(temp, slice);
    temp->remaining_burst -= slice;
    time += slice;
    printf("\tTime is now: %d\n", time);
    if(!temp->remaining_burst) {
      delete(&task_list, temp);
    }
  }
}