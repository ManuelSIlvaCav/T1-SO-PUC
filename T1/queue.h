#include "process.h"

struct queue {
  struct process **data;
  int size;
  int cap;
  // 0- no hay nad ejecutando 1-procesos siendo ejecutado
  int estado_SO;
  struct process*proc_actual;
};

typedef struct queue Queue;

Queue* initQueue(int cap);

void insert(Queue *queue, Process * proc);
void insert_base_pid(Queue *queue, Process * proc);
void heapify(Queue*queue, int index );
Process*GetNext(Queue*queue, int tiempo);
Process*ExtractMax(Queue*queue);
