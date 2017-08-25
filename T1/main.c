#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "queue.h"


void callback() {
  printf("Adios amigos\n");
  exit(0);
}



int main(int argc, char* argv[]) {

  signal(SIGINT, callback);
  //prueba de procesos y cola

  //Armado de procesos acorde al txt
  char name[256];
  int priority;
  int start_time;
  int n_a;
  int n_intervals;
  int t = 0;
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

    int cpu_status = 0;
    int stop_cpu = 0;

    Queue*queue = initQueue(n_proc);

    while(1) {

      printf("TIEMPO: %d\n", t);
      /* Revisamos si es que tenemos que desperatar un proceso */
      for(int i = 0; i < n_proc; i++) {
        stop_cpu = 1;
        Process *proc = procs[i];
        if (proc->status != 3) { // If not terminated
          // printf("Entramos\n");
          // printf("PID: %d Estatus :%d, awake: %d intervalind: %d fin: %d\n", proc->PID, proc->status, proc->awake_time, proc->interval_index, proc->interval_size);
          stop_cpu = 0;
          if (proc->status == 2 && proc->awake_time == t) {
            printf("El proceso PID: %d cambia a estado READY\n", proc->PID);
            proc->status = 0; // Process is ready;
            inser_fcfs(queue, proc);
            //Calculo REsponse y Turnarround
            if (proc->first_time == 0){
              proc->first_time = 1;
              proc->response = t;
              proc->turnarround = t;
            }
          }
        }
      }
        if (stop_cpu) {
          int contador = 0;
          for (int i = 0; i < n_proc; i ++){
            // printf("PROCESO %d status: %d :D\n", procs[i]->PID, procs[i]->status);
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
          // printf("La CPU esta en estado IDLE\n");
        } else {
          Process*process = extractMaxFcfs(queue);
          queue->proc_actual = process;
          printf("El proceso PID: %d cambia a estado RUNNING\n", process->PID);
          process->status = 1;
          // printf("A: %d\n", process->intervals[process->interval_index]);
          process->sleep_time = t + process->intervals[process->interval_index];
          process->interval_index += 1;
          cpu_status = 1;

          //Contador de ejecutadas
          process->seleccion_ejecutadas ++;
          //response
          if (process->first_time == 1){
            process->response = t - process->response;
            process->first_time = 2;
          }
        }
      }
      /* Si la cpu esta ejecutando algo, debemos revisar si es que termino */
      if (cpu_status == 1) {
        Process*process = queue->proc_actual;
        printf("Estamos ejecutando el proceso PID: %d\n", process->PID);
        // printf("Sleep: %d\n", process->sleep_time);
        //printf("Sleep: %d\n", process->sleep_time);


        if (process->sleep_time == t ) {
          // Revisamos si el proceso termina
          if (process->interval_index == process->interval_size) {
            printf("Detruimos el proceso PID: %d\n", process->PID);

            //Turnarround
            process->turnarround = t- process->turnarround;

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

            //COntador bloqueos
            process->bloqueos ++;
          }

        }

      }

      // Waiting TIME
      for (int i = 0; i < n_proc; i++){
        if (procs[i]->status== 0 || procs[i]->status == 2){
          procs[i]->t_waiting ++;
        }
      }
      /* Actalizamos el tiempo */
      t += 1;
      printf("---------\n");
    }



  }

  else if (strcmp(input, "priority") == 0){

    int cpu_status = 0;
    int stop_cpu = 0;
    Queue *queue = initQueue(n_proc);
    Process *process;
    while(1) {
      printf("TIEMPO: %d\n", t);
      /* Revisamos si es que tenemos que desperatar un proceso */
      for(int i = 0; i < n_proc; i++) {
        stop_cpu = 1;
        Process *proc = procs[i];
        if (proc->status != 3) { // If not terminated
          // printf("PID: %d Estatus :%d, awake: %d intervalind: %d fin: %d\n", proc->PID, proc->status, proc->awake_time, proc->interval_index, proc->interval_size);
          stop_cpu = 0;
          if (proc->status == 2 && proc->awake_time == t) {
            printf("El proceso PID: %d cambia a estado READY\n", proc->PID);
            proc->status = 0; // Process is ready;
            insert(queue, proc);
          }


          //Calculo REsponse y Turnarround
          if (proc->first_time == 0){
            proc->first_time = 1;
            proc->response = t;
            proc->turnarround = t;
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
        } else {
          process = extractMax(queue);
          printf("El proceso PID: %d cambia a estado RUNNING\n", process->PID);
          process->status = 1;
          printf("A: %d\n", process->intervals[process->interval_index]);
          process->sleep_time = t + process->intervals[process->interval_index];
          process->interval_index += 1;
          printf("El proceso PID: %d ha ejecutado %d intervalos de tiempo\n", process->interval_index);
          cpu_status = 1;

          process->seleccion_ejecutadas ++;

          //response
          if (process->first_time == 1){
            process->response = t - process->response;
            process->first_time = 2;
          }
        }
      }
      /* Si la cpu esta ejecutando algo, debemos revisar si es que termino */
      if (cpu_status == 1) {
        printf("Estamos ejecutando el proceso PID: %d\n", process->PID);
        // printf("Sleep: %d\n", process->sleep_time);
        //printf("Sleep: %d\n", process->sleep_time);


        if (process->sleep_time == t ) {
          // Revisamos si el proceso termina
          if (process->interval_index == process->interval_size) {
            printf("Detruimos el proceso PID: %d\n", process->PID);
            process->status = 3;
            //Turnarround
            process->turnarround = t- process->turnarround;
          }
          else {
            printf("El proceso PID: %d cambia a estado WAITING\n", process->PID);
            process->status = 2; // Process is waiting
            process->awake_time = t + process->intervals[process->interval_index];
            process->interval_index += 1;
            cpu_status = 0;

            process->bloqueos ++;
          }

        }

      }
      for (int i = 0; i < n_proc; i++){
        if (procs[i]->status== 0 || procs[i]->status == 2){
          procs[i]->t_waiting ++;
        }
      }
      /* Actalizamos el tiempo */
      t += 1;
    }
  }

  else if (strcmp(input, "roundrobin")==0){

    int quantum = 3;
    if (argv[3] != NULL) quantum = strtol(argv[3], NULL, 10);
    

    int cpu_status = 0;
    int stop_cpu = 0;

    Queue*queue = initQueue(n_proc);

    //ASIGNAMOS EL QUANTUM Y LA PRIORIDAD A roundrobin
    for (int i = 0; i < n_proc; i++){
      int prioridad = PrioridadQuamtum(procs[i]->priority, quantum);
      int qtime = Qtiempo(prioridad);
      // printf("QTIME Y PRIORIDAD %d, %d\n", prioridad, qtime);

      procs[i]->quantum_time = qtime;
      procs[i]->priority = prioridad;
    }

    //SIMULAMOS
    while(1) {

      printf("TIEMPO: %d\n", t);
      /* Revisamos si es que tenemos que desperatar un proceso */
      for(int i = 0; i < n_proc; i++) {
        stop_cpu = 1;
        Process *proc = procs[i];
        if (proc->status != 3) { // If not terminated

          // printf("PID: %d Estatus :%d, awake: %d intervalind: %d fin: %d, quantum: %d, A: %d\n", proc->PID, proc->status, proc->awake_time, proc->interval_index, proc->interval_size, proc->quantum_time, proc->intervals[proc->interval_index]);
          stop_cpu = 0;
          // 2 - WAITING
          if (proc->status == 2 && proc->awake_time == t) {
            printf("El proceso PID: %d cambia a estado READY\n", proc->PID);
            proc->status = 0; // Process is ready;
            inser_fcfs(queue, proc);
          }


          //Calculo REsponse y Turnarround
          if (proc->first_time == 0){
            proc->first_time = 1;
            proc->response = t;
            proc->turnarround = t;
          }

        }
      }
        if (stop_cpu) {
          int contador = 0;
          for (int i = 0; i < n_proc; i ++){
            // printf("PROCESO %d status: %d :D\n", procs[i]->PID, procs[i]->status);
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
        } else {
          Process*process = extractMaxFcfs(queue);
          queue->proc_actual = process;
          printf("El proceso PID: %d cambia a estado RUNNING\n", process->PID);
          process->status = 1;

          // elegido
          process->seleccion_ejecutadas ++;
          //SI Q >= A
          if (process->quantum_time >= process->intervals[process->interval_index]){
            // printf("A TIME: %d\n",process->quantum_time - process->intervals[process->interval_index]);
            process->sleep_time = t + (process->quantum_time - process->intervals[process->interval_index]);
            process->interval_index += 1;
            //CUANDO LE TOQUE 1- QUE SI DUERMA
            process->sleep_robin = 1;
            cpu_status = 1;
          }

          // Q < A
          else {
            printf("Q TIME: %d\n",process->quantum_time );
            process->sleep_time = t + process->quantum_time;
            //Sleep robin = 0 NO DORMIR PORQUE FALTA POR EJECUTAR
            process->sleep_robin = 0;
            cpu_status = 1;
          }

          //response
          if (process->first_time == 1){
            process->response = t - process->response;
            process->first_time = 2;
          }
        }
      }
      /* Si la cpu esta ejecutando algo, debemos revisar si es que termino */
      if (cpu_status == 1) {
        Process*process = queue->proc_actual;
        printf("Estamos ejecutando el proceso PID: %d\n", process->PID);
        // printf("Sleep: %d\n", process->sleep_time);
        //printf("Sleep: %d\n", process->sleep_time);


        if (process->sleep_time == t) {
          // Revisamos si el proceso termina
          if (process->interval_index == process->interval_size) {
            printf("Detruimos el proceso PID: %d\n", process->PID);
            //Turnarround
            process->turnarround = t- process->turnarround;

            for (int i = 0; i < queue->size; i++ ){
              printf("HAY proceso en cola %s \n", queue->data[i]->name);
            }
            process->status = 3;
            cpu_status = 0;
          }
          else {
            if (process->sleep_robin == 1){
              printf("El proceso PID: %d cambia a estado WAITING\n", process->PID);
              process->status = 2; // Process is waiting
              process->awake_time = t + process->intervals[process->interval_index];
              process->interval_index += 1;
              cpu_status = 0;
              process->sleep_robin = -1;

              //BLoqueos
              process->bloqueos ++;
            }

            else if (process->sleep_robin == 0){
              printf("El proceso PID: %d cambia a estado READY Q < A\n", process->PID);
              process->status = 0; // Process is READY
              process->intervals[process->interval_index] -= process->quantum_time;
              inser_fcfs(queue, process);
              cpu_status = 0;

            }

          }

        }

      }
      //WAITING
      for (int i = 0; i < n_proc; i++){
        if (procs[i]->status== 0 || procs[i]->status == 2){
          procs[i]->t_waiting ++;
        }
      }
      /* Actalizamos el tiempo */
      t += 1;
      printf("---------\n");
    }
  }



  FILE*f = fopen("Estadisticas.txt", "w");
  fprintf(f, "Estadisticas Generales:\n" );
  int contador_terminados = 0;
  for (int i = 0; i < n_proc; i++){
    if (procs[i]->status == 3) contador_terminados++;
  }
  fprintf(f,"Procesos Terminados: %d\n", contador_terminados);
  fprintf(f,"Tiempo SImulacion: %d\n\n", t);

  fprintf(f, "Estadisticas por Procesos:\n");
  for (int i = 0; i < n_proc; i++){
    fprintf(f, "Proceso: %d \n Numero Veces Elegido Ejecutar: %d\n Numero Bloqueos: %d\n", procs[i]->PID, procs[i]->seleccion_ejecutadas, procs[i]->bloqueos);
    fprintf(f, "Waiting Time: %d\n", procs[i]->t_waiting);
    fprintf(f, "Response Time: %d\n Turnarround Time: %d\n\n", procs[i]->response, procs[i]->turnarround);
  }



  return 0;
}
