// Olga Kuriatnyk
// 5/11/2021
// CSS 430
// P3: Scheduling Algorithms
// list.h
// list data structure containing the tasks in the system
// This class was modified
 

#include <stdbool.h>
#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
struct node* insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);

bool comesBefore(char *a, char *b);
float utilizationCPU(int totalTime, int dispatcherTime);
void tableOfInfo(int size, struct node *list1, struct node *list2, struct node *list3, struct node *list4);