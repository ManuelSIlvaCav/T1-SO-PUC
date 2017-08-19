#include "queue.h"
#include <math.h>

Queue* initQueue(int cap) {
  Queue *new = calloc(1, sizeof(Queue));
  new->cap = cap;
  new->size = 0;
  new->estado_SO = 0;
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

int left(int i){
  return (2*i +1);
}

int right(int i){
  return (2*i +2);
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

void insert_base_pid(Queue *queue, Process * proc) {
  queue->size += 1;
  int i = queue->size - 1;
  queue->data[i] = proc;



  while(i != 0 && queue->data[parent(i)]->PID < queue->data[i]->PID) {
    printf("%d %d \n", parent(i), i);
    swap(queue, i, parent(i));
    i = parent(i);
  }


}


void heapify(Queue*queue, int index){
  int l = left(index);
  int r = right(index);
  int max = index;
  if (l < queue->size && queue->data[l]->priority > queue->data[index]->priority){
    max = l;
  }
  if (r < queue->size && queue->data[r]->priority > queue->data[max]->priority){
    max = r;
  }

  if (max != index){
    swap(queue, index, max);
    heapify(queue, max);
  }

}

Process*ExtractMax(Queue*queue){
  if (queue->size == 1){
    queue->size --;
    return queue->data[0];
  }

  Process* root = queue->data[0];
  queue->data[0] = queue->data[queue->size-1];
  queue->size --;
  heapify(queue, 0);
  return root;
}

void decreaseKey(Queue*queue, int index, int value){
  queue->data[index]->priority = value;
  while (index!= 0 && queue->data[parent(index)] > queue->data[index]){
    swap(queue, index, parent(index));
    index = parent(index);
  }
}

Process*GetNext(Queue*queue, int tiempo_actual){
  for (int i =0; i < queue->size; i++){
    if (queue->data[i]->status == 0 && queue->data[i]->tiempo_inicio <= tiempo_actual){
      return queue->data[i];
    }

  }
}
