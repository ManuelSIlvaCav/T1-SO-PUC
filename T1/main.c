#include <stdio.h>
#include "queue.h"

int main() {
  Process* a = initProc(1, "Uno", 12, 0);
  Process* b = initProc(2, "dos", 1, 0);
  Process* c = initProc(3, "tres", 4, 0);
  Process* d = initProc(4, "cuatro", 7, 0);

  Queue* q = initQueue(100);
  insert(q, a);
  insert(q, b);
  insert(q, c);
  insert(q, d);

  for (int i = 0; i < 4; i++) {
    Process * current = q->data[i];
    printf("p:%d ID: %d\n", current->priority, current->PID);
  }


  return 0;
}