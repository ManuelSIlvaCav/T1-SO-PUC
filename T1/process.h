#include <stdlib.h>

struct process{
  int PID;
  char* name;
  int priority;
  // 0- Ready, 1-Running 2-waiting 3 -dead
  int status;
  int *array;
  int tiempos;
  int tiempo_inicio;
  int indice_arreglo_actual;
  int tiempo_termino_ejecucion;
  int quantum_robin;

};
typedef struct process Process;


Process *initProc(int id, char* name, int priority, int tiempo_inicio, int status, int tiempos, int* lista_tiempos);

Process*ExtractMin();

void actualizar_proceso(int tiempo_actual, Process*proc);
void actualizar_proc_robin(int tiempo_actual, Process*proc);
