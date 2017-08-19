#include "process.h"

Process *initProc(int id, char* name, int priority, int tiempo_inicio, int status, int tiempos, int *lista_tiempos) {
  Process *new = malloc(sizeof(Process));
  new->PID = id;
  new->name = name;
  new->priority = priority;
  new->status = status;
  new->tiempo_inicio = tiempo_inicio;
  new->tiempos = tiempos;
  new->indice_arreglo_actual =0;
  new->tiempo_termino_ejecucion=0;

  new->array = (int*)malloc(sizeof(int) * (tiempos*2-1));
  for(int i = 0; i<tiempos*2-1; i++) new->array[i] = lista_tiempos[i];
  //printf("%d dadasd \n", lista_tiempos[1]);
  return new;
}


void actualizar_proceso(int tiempo_actual, Process*proc){
  proc->tiempo_termino_ejecucion = proc->tiempo_inicio + proc->array[proc->indice_arreglo_actual];
  proc->status = 1;
}
