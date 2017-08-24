#include "queue.h"
#include <math.h>

Queue* initQueue(int cap) {
  Queue *new = calloc(1, sizeof(Queue));
  new->cap = cap;
  new->size = 0;
  new->estado_SO = 0;
  new->data = calloc(cap, sizeof(Process*));
  new->indice_robin = 0;
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
      if (queue->data[i]->indice_arreglo_actual == 0) printf("CREE EL PROCESO %d\n", queue->data[i]->PID);
      else printf("CONSEGUI NEXT %d Proceso: %d ", tiempo_actual, queue->data[i]->PID);
      queue->estado_SO = 1;
      queue->data[i]->tiempo_inicio = tiempo_actual;
      queue->proc_actual = queue->data[i];
      return queue->data[i];
    }

  }
  return NULL;
}

void LiberarWaiting(Queue*queue, int tiempo_actual){
  for (int i = 0; i < queue->size; i++){
    if (queue->data[i]->status == 2 && queue->data[i]->tiempo_inicio >= tiempo_actual){
      queue->data[i]->status = 0;
    }
  }
}

void EndProc(Queue*queue, int tiempo_actual){
  //SI el proceso siendo ejecutado termino su ejecucion
  if (queue->proc_actual->tiempo_termino_ejecucion <= tiempo_actual){

    //estado Waiting = 2
    queue->proc_actual->status = 2;
    //proximo inicio -> Termino + Bi
    queue->proc_actual->tiempo_inicio = queue->proc_actual->array[queue->proc_actual->indice_arreglo_actual+1] + queue->proc_actual->tiempo_termino_ejecucion;
    queue->proc_actual->indice_arreglo_actual += 2;

    //Si me paso del numero de tiempos- Mato el proceso
    if (queue->proc_actual->tiempos < queue->proc_actual->indice_arreglo_actual) {
      queue->proc_actual->status=3;
      printf("Mate al proceso %d, %s\n", queue->proc_actual->PID, queue->proc_actual->name);
      }
    queue->estado_SO = 0;
    printf("TERMINE ACTUAL en %d proceso: %d proximo:%d\n", tiempo_actual, queue->proc_actual->PID, queue->proc_actual->tiempo_inicio);
  }
}


int potencia(int base, int expo){
  if (expo == 0) return 1;
  else if (expo%2) return base * potencia(base, expo-1);
  else {
    int temp = potencia(base, expo/2);
    return temp * temp;
  }
}
int PrioridadQuantum(int prioridad_tex, int quantum){
  int prioridad = 0;
  double divi = prioridad_tex/quantum;
  prioridad = (prioridad_tex*quantum) + potencia(-1, round(divi))*prioridad_tex;
  return prioridad;
}

int Qtiempo(int prioridad){
  double result;
  result = ceil(prioridad/64);
  return result;
}

Process*RobinNext(Queue*queue, int tiempo_actual){
  for(int i = 0; i < queue->size; i++){
    if (queue->data[i]->status == 0 && queue->data[i]->tiempo_inicio <= tiempo_actual){
      queue->estado_SO = 1;
      queue->data[i]->tiempo_inicio = tiempo_actual;
      queue->proc_actual = queue->data[i];
      return queue->data[i];
    }
  }
}

void RoundRobin(Queue*queue, int tiempo_actual){
  //No hay nada ejecutando busco al proximo
  if (queue->estado_SO == 0){
    Process* proc = RobinNext(queue, tiempo_actual);
    if (proc == NULL);
    else {

        actualizar_proceso(tiempo_actual, proc);
    }


  }
}
