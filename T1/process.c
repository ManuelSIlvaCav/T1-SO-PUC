#include "process.h"

Process *initProc(int id, char* name, int priority, int status) {
  Process *new = malloc(sizeof(Process));
  new->PID = id;
  new->name = name;
  new->priority = priority;
  new->status = status;
  return new;
}