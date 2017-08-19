#include "process.h"

Process *initProc(int id, char* name, int priority, int tiempo_inicio, int status, int tiempos, int*lista_tiempos) {
  Process *new = malloc(sizeof(Process));
  new->PID = id;
  new->name = name;
  new->priority = priority;
  new->status = status;
  new->tiempo_inicio = tiempo_inicio;
  new->tiempos = tiempos;
  new->array = lista_tiempos;
  new->indice_arreglo_actual =0;
  return new;
}
