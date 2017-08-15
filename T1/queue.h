#include "process.h"

struct queue {
  struct process **data;
  int size;
  int cap;
};

typedef struct queue Queue;

Queue* initQueue(int cap);

void insert(Queue *queue, Process * proc);

void heapify(Queue*queue, int index );

Process*ExtractMax(Queue*queue);
