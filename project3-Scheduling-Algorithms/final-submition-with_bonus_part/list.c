// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// list.c
// Various list operations
// This class was modified

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"

// add a new task to the list of tasks
// return new Node
struct node* insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
    return newNode;
}

// delete the selected task from the list
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}

// calculete CPU utilization
float utilizationCPU(int totalTime, int dispatcherTime) {
  float result = (totalTime / (float)dispatcherTime) * 100;
  printf("\nCPU Utilization: %.2f%%\n", result);
  return result;
}

// print the table of information
void tableOfInfo(int size, struct node *list1, struct node *list2, struct node *list3, struct node *list4) {
  struct node *temp1 = list1;
  int tempSize = size;
  printf("\n...|");
  while (tempSize > 0) {
    Task *best_sofar = temp1->task;
    while (temp1) { 
      if (comesBefore(temp1->task->name, best_sofar->name)) {
        best_sofar = temp1->task;
      }
      temp1 = temp1->next;
    }
    printf(" %4s |", best_sofar->name);
    delete(&list1, best_sofar);
    temp1 = list1;
    tempSize--;
  }

  struct node *temp2 = list2;
  tempSize = size;
  printf("\nTAT|");
  while (tempSize > 0) {
    Task *best_sofar = temp2->task;
    while (temp2) { 
      if (comesBefore(temp2->task->name, best_sofar->name)) {
        best_sofar = temp2->task;
      }
      temp2 = temp2->next;
    }
    printf(" %4i |", best_sofar->tat);
    delete(&list2, best_sofar);
    temp2 = list2;
    tempSize--;
  }

  struct node *temp3 = list3;
  tempSize = size;
  printf("\nWT |");
  while (tempSize > 0) {
    Task *best_sofar = temp3->task;
    while (temp3) { 
      if (comesBefore(temp3->task->name, best_sofar->name)) {
        best_sofar = temp3->task;
      }
      temp3 = temp3->next;
    }
    printf(" %4i |", best_sofar->wt);
    delete(&list3, best_sofar);
    temp3 = list3;
    tempSize--;
  }

  struct node *temp4 = list4;
  tempSize = size;
  printf("\nRT |");
  while (tempSize > 0) {
    Task *best_sofar = temp4->task;
    while (temp4) { 
      if (comesBefore(temp4->task->name, best_sofar->name)) {
        best_sofar = temp4->task;
      }
      temp4 = temp4->next;
    }
    printf(" %4i |", best_sofar->rt);
    delete(&list4, best_sofar);
    temp4 = list4;
    tempSize--;
  }
  printf("\n\n");
}