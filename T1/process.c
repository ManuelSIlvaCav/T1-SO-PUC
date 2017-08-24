#include "process.h"

/*
0: READY,
1: RUNNING,
2: WAITING,
*/


Process *initProc(int id, char* name, int priority, int start_time, int interval_size, int *intervals) {
  printf("Creamos el proceso PID: %d, %d\n", id, start_time);
  Process *new = malloc(sizeof(Process));
  new->PID = id;
  new->name = name;
  new->priority = priority;
  new->status = 2;



  // new->tiempos = tiempos;
  // new->indice_arreglo_actual =0;
  // new->tiempo_termino_ejecucion=0;

  //new->intervals = intervals;
  new->interval_size = interval_size;
  new->interval_index = 0;

  new->awake_time = start_time;
  new->sleep_time = 0;


  new->intervals = (int*)malloc(sizeof(int) * (interval_size));
  for(int i = 0; i<interval_size; i++) {
    new->intervals[i] = intervals[i];
    printf("%d dadasd \n", new->intervals[i]);
  }
  return new;
}

/*
void actualizar_proceso(int tiempo_actual, Process*proc){
  proc->tiempo_termino_ejecucion = proc->tiempo_inicio + proc->array[proc->indice_arreglo_actual];
  proc->status = 1;
}
*/
