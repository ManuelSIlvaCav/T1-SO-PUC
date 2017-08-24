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
void inser_fcfs(Queue*queue, Process*proc);
void heapify(Queue*queue, int index );
Process*GetNext(Queue*queue, int tiempo);
Process* extractMax(Queue*queue);
Process*extractMaxFcfs(Queue*queue);

void LiberarWaiting(Queue*queue);
void EndProc(Queue*queue, int tiempo_actual);

int isEmpty(Queue *queue);

int potencia(int base, int expo);
