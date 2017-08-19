#include <stdio.h>
#include "queue.h"

int main(int argc, char * argv[]) {

  //prueba de procesos y cola

  //Armado de procesos acorde al txt
  char nombre[256];
  int prioridad;
  int tiempo_inicio;
  int largo_tiempos;
  int PID = 99999;

  char*txt = argv[2];
  FILE*fr;
  fr = fopen(txt, "r");
  Queue* q = initQueue(100);


  //algoritmos

  char*input = argv[1];
  int i=0;
  if (strcmp(input, "fcfs")==0){
    printf("Estoy en fcfs\n");

    /*Codigo para lectura de archivos */
    while(fscanf(fr, "%s", nombre) != EOF){
      fscanf(fr, "%d %d %d", &prioridad, &tiempo_inicio, &largo_tiempos);
      printf("Printeando %s, %d, %d, %d fin\n", nombre, prioridad, tiempo_inicio, largo_tiempos);
      int lista_tiempo[largo_tiempos*2-1];
      for (int i=0; i< largo_tiempos*2-1; i++){
        fscanf(fr, "%d\n", &lista_tiempo[i]);

      }
      Process*proc = initProc(PID, nombre, prioridad, tiempo_inicio, 0, largo_tiempos, lista_tiempo);
      //printf("p:%d ID: %d, da %s anterior\n", proc->priority, proc->PID, proc->name);
      //insert especial que inserta en base al id y no prioridad -- SE VAN CREANDO CON ID DECRECIENTES
      insert_base_pid(q, proc);

      PID--;

    }

    //Se "ordena" el heap mediante el id del procesos, ya que se selecciona en el orden que entran no la prioridad

    for (int i = 0; i < q->size; i++) {
      Process * current = q->data[i];
      printf("p:%d ID: %d, da %s %d %d\n", current->priority, current->PID, current->name, current->tiempo_inicio, current->tiempos);
      for (int indice = 0; indice<current->tiempos*2-1; indice++) printf("%d dasd\n", current->array[indice]);
    }

    while(1){
      //si el SO esta desocupado empezar a ejecutar
      if (q->estado_SO==0){
        Process*Ejecutado = GetNext(q, i);
        if (Ejecutado == NULL) printf("IDLE\n");
        else {
            printf("GetNext\n");
        }
        //printf("%d\n", Ejecutado->indice_arreglo_actual);
        //actualizar_proceso(i, Ejecutado);
      }
      //Revisar procesos waiting - Ready


      //chquear el termino del proceso actual - liberar SO
      //EndProc(q, i);


      i++;
      if (i==10000) break;
    }

  }
  else if (strcmp(input, "priority")== 0){
    printf("Estoy en priority");
  }

  else if (strcmp(input,"roundrobin")==0){
    printf("Estoy en roundrobin");
  }




  return 0;
}
