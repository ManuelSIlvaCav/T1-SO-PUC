#include <stdio.h>
#include <string.h>
#include "queue.h"

int main(int argc, char* argv[]) {

  //prueba de procesos y cola

  //Armado de procesos acorde al txt
  char name[256];
  int priority;
  int start_time;
  int n_a;
  int n_intervals;
  // int PID = 0;

  char*txt = argv[2];
  FILE*fr;
  fr = fopen(txt, "r");
  // Queue* q = initQueue(100);


  int n_proc = 0;
  while(fscanf(fr, "%s", name) != EOF) {
    n_proc += 1;
    fscanf(fr, "%d %d %d", &priority, &start_time, &n_a);

    int intervals[2 * n_a - 1];
    for (int i = 0; i < 2 * n_a - 1; i++) {
      fscanf(fr, "%d\n", &intervals[i]);
    }
  }

  fclose(fr);
  fr = fopen(txt, "r");

  Process ** procs = calloc(n_proc, sizeof(Process*));

  for (int i = 0; i < n_proc; i++) {
    fscanf(fr, "%s %d %d %d", name, &priority, &start_time, &n_a);
    n_intervals = 2 * n_a - 1;
    int intervals[n_intervals];
    for (int j = 0; j < n_intervals; j++) {
      fscanf(fr, "%d\n", &intervals[j]);
    }
    procs[i] = initProc(i, name, priority, start_time, n_intervals, intervals);
  }





  //algoritmos

  char* input = argv[1];
  // int i = 0;
  if (strcmp(input, "fcfs")==0){
    int t = 0;
    int cpu_status = 0;
    int stop_cpu = 0;

    Queue*queue = initQueue(100);

    while(t < 30) {

      printf("TIEMPO: %d\n", t);
      /* Revisamos si es que tenemos que desperatar un proceso */
      for(int i = 0; i < n_proc; i++) {
        stop_cpu = 1;
        Process *proc = procs[i];
        if (proc->status != 3) { // If not terminated
          // printf("Entramos\n");
          printf("PID: %d Estatus :%d, awake: %d intervalind: %d fin: %d\n", proc->PID, proc->status, proc->awake_time, proc->interval_index, proc->interval_size);
          stop_cpu = 0;
          if (proc->status == 2 && proc->awake_time == t) {
            printf("El proceso PID:%d cambia a estado READY\n", proc->PID);
            proc->status = 0; // Process is ready;
            inser_fcfs(queue, proc);
          }
        }
      }
        if (stop_cpu) {
          int contador = 0;
          for (int i = 0; i < n_proc; i ++){
            printf("PROCESO %d status: %d :D\n", procs[i]->PID, procs[i]->status);
            if (procs[i]->status != 3) contador++;

          }
          if (contador == 0){
            printf("Todos los procesos han terminado \n");
            break;
          }
        }

      
      /* Si la cpu no tiene proceso, sacamos uno de la cola*/
      if (cpu_status == 0) {
        if (isEmpty(queue)) {
          printf("No hay procesos en la cola READY\n");
          printf("La CPU esta en estado IDLE\n");
        } else {
          Process*process = extractMaxFcfs(queue);
          queue->proc_actual = process;
          printf("El proceso PID: %d cambia a estado RUNNING\n", process->PID);
          process->status = 1;
          printf("A: %d\n", process->intervals[process->interval_index]);
          process->sleep_time = t + process->intervals[process->interval_index];
          process->interval_index += 1;
          cpu_status = 1;
        }
      }
      /* Si la cpu esta ejecutando algo, debemos revisar si es que termino */
      if (cpu_status == 1) {
        Process*process = queue->proc_actual;
        printf("Estamos ejecutando el proceso PID: %d\n", process->PID);
        printf("Sleep: %d\n", process->sleep_time);
        //printf("Sleep: %d\n", process->sleep_time);


        if (process->sleep_time == t ) {
          // Revisamos si el proceso termina
          if (process->interval_index == process->interval_size) {
            printf("Detruimos el proceso PID: %d\n", process->PID);

            for (int i = 0; i < queue->size; i++ ){
              printf("HAY proceso en cola %s \n", queue->data[i]->name);
            }
            process->status = 3;
            cpu_status = 0;
          }
          else {
            printf("El proceso PID: %d cambia a estado WAITING\n", process->PID);
            process->status = 2; // Process is waiting
            process->awake_time = t + process->intervals[process->interval_index];
            process->interval_index += 1;
            cpu_status = 0;
          }

        }

      }
      /* Actalizamos el tiempo */
      t += 1;
      printf("---------\n");
    }



  }

  else if (strcmp(input, "priority") == 0){
    int t = 0;
    int cpu_status = 0;
    int stop_cpu = 0;
    Queue *queue = initQueue(100);
    Process *process;
    while(t < 30) {
      printf("TIEMPO: %d\n", t);
      /* Revisamos si es que tenemos que desperatar un proceso */
      for(int i = 0; i < n_proc; i++) {
        stop_cpu = 1;
        Process *proc = procs[i];
        if (proc->status != 3) { // If not terminated
          // printf("Entramos\n");
          printf("PID: %d Estatus :%d, awake: %d intervalind: %d fin: %d\n", proc->PID, proc->status, proc->awake_time, proc->interval_index, proc->interval_size);
          stop_cpu = 0;
          if (proc->status == 2 && proc->awake_time == t) {
            printf("El proceso PID:%d cambia a estado READY\n", proc->PID);
            proc->status = 0; // Process is ready;
            insert(queue, proc);
          }
        }
        if (stop_cpu) {
          printf("Todos los procesos han terminado \n");
          break;
        }
      }
      /* Si la cpu no tiene proceso, sacamos uno de la cola*/
      if (cpu_status == 0) {
        if (isEmpty(queue)) {
          printf("No hay procesos en la cola READY\n");
          printf("La CPU esta en estado IDLE\n");
        } else {
          process = extractMax(queue);
          printf("El proceso PID: %d cambia a estado RUNNING\n", process->PID);
          process->status = 1;
          printf("A: %d\n", process->intervals[process->interval_index]);
          process->sleep_time = t + process->intervals[process->interval_index];
          process->interval_index += 1;
          cpu_status = 1;
        }
      }
      /* Si la cpu esta ejecutando algo, debemos revisar si es que termino */
      if (cpu_status == 1) {
        printf("Estamos ejecutando el proceso PID: %d\n", process->PID);
        printf("Sleep: %d\n", process->sleep_time);
        //printf("Sleep: %d\n", process->sleep_time);


        if (process->sleep_time == t ) {
          // Revisamos si el proceso termina
          if (process->interval_index == process->interval_size) {
            printf("Detruimos el proceso PID: %d\n", process->PID);
            process->status = 3;
          }
          else {
            printf("El proceso PID: %d cambia a estado WAITING\n", process->PID);
            process->status = 2; // Process is waiting
            process->awake_time = t + process->intervals[process->interval_index];
            process->interval_index += 1;
            cpu_status = 0;
          }

        }

      }
      /* Actalizamos el tiempo */
      t += 1;
    }
  }

  else if (strcmp(input,"roundrobin")==0){
    printf("Estoy en roundrobin");
  }




  return 0;
}

void print_status(int status) {
  if (status) printf("La CPU está Trabajando");
  printf("IDLE");
}
