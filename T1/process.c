#include "process.h"

Process *initProc(int id, char* name, int priority, int status, int tiempos, int*lista_tiempos) {
  Process *new = malloc(sizeof(Process));
  new->PID = id;
  new->name = name;
  new->priority = priority;
  new->status = status;
  new->tiempos = tiempos;
  new->array = lista_tiempos;
  return new;
}
