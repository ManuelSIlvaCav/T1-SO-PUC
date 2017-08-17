#include <stdlib.h>

struct process{
  int PID;
  char* name;
  int priority;
  int status;
  int* array;
  int tiempos;
};
typedef struct process Process;


Process *initProc(int id, char* name, int priority, int status, int tiempos, int*lista_tiempos);

Process*ExtractMin();
