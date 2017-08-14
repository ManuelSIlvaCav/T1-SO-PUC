#include "queue.h"
#include <math.h>

Queue* initQueue(int cap) {
  Queue *new = calloc(1, sizeof(Queue));
  new->cap = cap;
  new->size = 0;
  new->data = calloc(cap, sizeof(Process*));
  for (int i = 0; i < cap; i++) {
    new->data[i] = calloc(1, sizeof(Process));
  }
  return new;
}

int parent(int i) {
  if (i == 0) return 0;
  return (i - 1) / 2;
}

void swap(Queue *queue, int i, int j) {
  Process *temp = queue->data[i];
  queue->data[i] = queue->data[j];
  queue->data[j] = temp;
}

void insert(Queue *queue, Process * proc) {
  queue->size += 1;
  int i = queue->size - 1;
  queue->data[i] = proc;


  
  while(i != 0 && queue->data[parent(i)]->priority < queue->data[i]->priority) {
    printf("%d %d \n", parent(i), i);
    swap(queue, i, parent(i));
    i = parent(i);
  }
  
  
}
