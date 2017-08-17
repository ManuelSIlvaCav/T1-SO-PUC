#include <stdio.h>
#include "queue.h"

int main(int argc, char * argv[]) {
  char*input = argv[1];
  if (strcmp(input, "fcfs")==0){
    printf("Estoy en fcfs\n");

  }
  else if (strcmp(input, "priority")== 0){
    printf("Estoy en priority");
  }

  else if (strcmp(input,"roundrobin")==0){
    printf("Estoy en roundrobin");
  }
  int a1[5] = {10, 11, 40, 40, 50};
  Process* a = initProc(1, "Uno", 12, 0, 5, a1);
  Process* b = initProc(2, "dos", 1, 0,1, a1);
  Process* c = initProc(3, "tres", 4, 0, 1, a1);
  Process* d = initProc(4, "cuatro", 7, 0, 1, a1);
  for (int i=0; i < a->tiempos; i++){
    printf("%d holi\n", a->array[i]);
  }
  Queue* q = initQueue(100);
  insert(q, a);
  insert(q, b);
  insert(q, c);
  insert(q, d);

  for (int i = 0; i < q->size; i++) {
    Process * current = q->data[i];
    printf("p:%d ID: %d\n", current->priority, current->PID);
  }

  Process* prueba = ExtractMax(q);
  printf("jdkasdjaksdj \n" );

  for (int i = 0; i < q->size; i++) {
    Process * current = q->data[i];
    printf("p:%d ID: %d\n", current->priority, current->PID);
  }

  return 0;
}
