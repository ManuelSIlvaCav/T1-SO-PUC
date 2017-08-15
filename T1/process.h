#include <stdlib.h>

struct process{
  int PID;
  char* name;
  int priority;
  int status;
  int* array;
};
typedef struct process Process;


Process *initProc(int id, char* name, int priority, int status);

Process*ExtractMin();
