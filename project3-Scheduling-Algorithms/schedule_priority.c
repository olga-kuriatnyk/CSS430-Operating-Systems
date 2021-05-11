#include<string.h>
#include<stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list = NULL;

// add a task to the list
void add(char *name, int priority, int burst) {
  Task *temp = malloc(sizeof(Task));
  // allocate memory and then copy the name
  temp->name = malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(temp->name, name);
  // priority and burst
  temp->priority = priority;
  temp->burst = burst;
  // insert into task list
  insert(&task_list, temp);
}

// pick the next task to execute with the Highest Priority
// for task with the same priority chose in lexicographical order.
Task *pickNextTask() {
  if (!task_list) {
    return NULL;
  }
  Task *highest_priority_job = task_list->task;
  struct node *n = task_list;
  while(n){
    if(n->task->priority >= highest_priority_job->priority){
      highest_priority_job = n->task;
    }
    n = n->next;
  }
  return highest_priority_job;
}

// invoke the scheduler
void schedule() {
  while(task_list) {
    int time = 0;
    Task *temp = pickNextTask();
    run(temp, temp->burst);
    time += temp->burst;
    printf("\tTime is now: %d\n", time);
    delete(&task_list, temp);
  }
}