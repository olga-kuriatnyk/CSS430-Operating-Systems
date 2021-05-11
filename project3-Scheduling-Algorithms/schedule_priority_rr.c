#include<string.h>
#include<stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *task_list[MAX_PRIORITY + 1];
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
  insert(&task_list[priority], temp);
}

// pick the next task to execute with Priority + Round Robin
Task *pickNextTask(struct node *tl) {
  // if (!task_list) {
  //   return NULL;
  // }
  Task *ret = next_node->task;
  next_node = (next_node -> next) ? next_node->next : tl;
  return ret;
}

// invoke the scheduler
void schedule() {
  int time = 0;
  // from higher priority to lower priority
  for(size_t p = MAX_PRIORITY; p >= MIN_PRIORITY; p--) {
    next_node = task_list[p];
    while(task_list[p]) {
      Task *t = pickNextTask(task_list[p]);
      int slice = QUANTUM < t->remaining_burst ? QUANTUM : t->remaining_burst;
      run(t, slice);
      t->remaining_burst -= slice;
      time += slice;
      printf("\tTime is now: %d\n", time);
      if(!t->remaining_burst) {
        delete(&task_list[p], t);
      }
    }
  }
}