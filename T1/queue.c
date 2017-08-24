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


/*Lo metemos alfinal de la cola en fcfs, orden de llegada*/
void inser_fcfs(Queue*queue, Process*proc){
  queue->size += 1;
  int i = queue->size - 1;
  queue->data[i] = proc;

  printf("PRINTEANDO COLA INSERT\n");
  for (int i = 0; i < queue->size; i ++){
    printf("POS %d, PROCESO %d\n", i, queue->data[i]->PID);
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

Process *extractMax(Queue*queue){
  if (queue->size == 0) return NULL;

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

Process*extractMaxFcfs(Queue*queue){
  if (queue->size == 0) return NULL;

  if (queue->size == 1){
    queue->size --;
    return queue->data[0];
  }

  Process* root = queue->data[0];
  for(int i =1; i < queue->size; i++ ){
    queue->data[i-1] = queue->data[i];
  }
  queue->size --;
  printf("PRINTEANDO COLA EXTRACT\n");
  for (int i = 0; i < queue->size; i ++){
    printf("POS %d, PROCESO %d\n", i, queue->data[i]->PID);
  }
  return root;
}




void decreaseKey(Queue*queue, int index, int value){
  queue->data[index]->priority = value;
  while (index!= 0 && queue->data[parent(index)] > queue->data[index]){
    swap(queue, index, parent(index));
    index = parent(index);
  }
}




int isEmpty(Queue *queue) {
  return queue->size == 0;
}




int potencia(int base, int expo){
  if (expo == 0)return 1;
  else if (expo%2) return base*potencia(base, expo-1);
  else{
    int temp = potencia(base, expo/2);
    return temp*temp;
  }
}

int PrioridadQuamtum(int prioridad_txt, int quantum){
  int prioridad = 0;
  double divi = prioridad_txt/quantum;
  prioridad = (prioridad_txt*quantum) + potencia(-1, round(divi))*prioridad_txt;
  return prioridad;
}

int Qtiempo(int prioridad){
  double result;
  result = ceil(prioridad/64);
  return result;
}
