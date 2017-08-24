#include <stdlib.h>

/*
0: READY,
1: RUNNING,
2: WAITING,
3: TERMINATED,
*/

struct process{
  int PID;
  char* name;
  int priority;
  int status;

  int *intervals;
  int interval_size;
  int interval_index;

  int quantum_time;
  int sleep_robin;
  int awake_time;
  int sleep_time;

};
typedef struct process Process;


Process *initProc(int id, char* name, int priority, int start_time, int intervals, int* lista_tiempos);

Process*ExtractMin();

void actualizar_proceso(int tiempo_actual, Process*proc);
