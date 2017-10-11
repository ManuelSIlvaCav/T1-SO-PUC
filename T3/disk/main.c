#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void fcfs(int head, int n, int *accesos, int*tupla);
void sstf(int head, int n, int*accesos, int*tupla);
int* SCAN(int head, int n, int max, int*accesos, int*tupla);
int*CLOOK(int head, int n, int*accesos, int*tupla);
void insertion_sort(int n, int*arr);
void rvereseArray(int*arr, int left, int right);
int IndexMin(int*arr, int n, int checkm, int*indexlist);
int checkindex(int*arr, int*indexarr, int n, int index);

int checkindex(int*arr, int*indexarr, int n, int index){
  int valor =0;
  for (int i =0; i < n; i++){
    if (indexarr[i] == index) valor = 1;
  }
  return valor;
}


int IndexMin(int*arr, int n, int check, int* indexlist){
  int index;
  int min = 999999999;
  for (int i = 0; i < n; i++){
    if (abs(arr[i] -check) < min && checkindex(arr, indexlist, n, i)==0){
      min = abs(arr[i]-check);
      index = i;
    }
  }
  return index;
}

void insertion_sort(int n, int*arr){
  int i, key, j;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j] > key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}

void rvereseArray(int*arr, int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}


//se retorna una tupla de la forma [Cantidad Despl, tiempo desplazamientos]
void fcfs(int head, int n, int*accesos, int*tupla){
  //Realizamos la cola igual como viene...
  int direcciones = 0;
  int cilindros = 0;
  int actual = head;
  int dir_inicial;
  int new_dir;

  for (int i =0; i < n; i++){
    if (i ==0){
      //Si next es < head == 0
      if (accesos[i] < head) dir_inicial = 0;
      else if(accesos[i] > head) dir_inicial = 1;
      else dir_inicial = -1;
      //printf("%d dir inicial;\n", dir_inicial);
    }

    else {
      //DIR INICIAL NO igual
      if (dir_inicial != -1){
        if (accesos[i] < actual) new_dir = 0;
        else if(accesos[i] > actual) new_dir = 1;
        else new_dir =2;
      }

      //DIR INICIAL mismo ==-1
      else {
        if (accesos[i] < actual) {
          new_dir = 0;
          dir_inicial = new_dir;
        }
        else if(accesos[i] > actual){
          new_dir = 1;
          dir_inicial = new_dir;
        }

        else {
           new_dir =2;
           dir_inicial = -1;
             }
      }

      if (new_dir != dir_inicial && new_dir!=2 && dir_inicial != -1){
        direcciones = direcciones + 1;
        dir_inicial = new_dir;
      }


      }
    printf("VOY DESDE %d a %d, seek: %d direcciones %d\n", actual, accesos[i], abs(actual - accesos[i]), direcciones );
    cilindros += abs(actual - accesos[i]);
    actual = accesos[i];
  }

  //printf("Cilindros %d\n", cilindros);
  tupla[0] = cilindros;
  tupla[1] = direcciones;
}

void sstf(int head, int n, int*accesos, int*tupla){

  int temp;
  int *t = calloc(n, sizeof(int));
  int *indexlist = calloc(n, sizeof(int));
  int contador = 0;
  int actual_head = head;
  for(int i =0; i < n; i++){
    indexlist[i] = -1;
    t[i] = accesos[i];
  }
  for (int i = 0; i < n; i++){
    temp = IndexMin(accesos, n, actual_head, indexlist);
    indexlist[contador] = temp;
    contador++;
    actual_head = accesos[temp];
  }



  for (int i = 0; i <n; i++){
    accesos[i] = t[indexlist[i]];
  }

  // for (int i = 0; i < n; i++){
  //   t[i] = abs(head - accesos[i]);
  // }
  //
  //
  //
  // for (int i = 0; i < n; i++){
  //   for (int j = i+1; j <n; j++){
  //     if (t[i] > t[j]){
  //       temp = t[i];
  //       t[i] = t[j];
  //       t[j] = temp;
  //       temp = accesos[i];
  //       accesos[i] = accesos[j];
  //       accesos[j] = temp;
  //     }
  //   }
  // }

  //  for(int i = 0; i <n; i++){
  //    printf("loop ue %d, %d\n", t[i], accesos[i]);
  //  }


  free(t);
  free(indexlist);
  


}
//elevador parte para arriba
int* SCAN(int head, int n, int maximal, int* accesos, int*tupla){
  int mayores = 0;
  int menores = 0;
  for (int i = 0; i < n; i++){
    if (accesos[i] >= head) mayores = mayores +1;
    else menores = menores + 1;
  }

  //printf("%d %d\n", mayores, menores);
  //le agregamos la ida al 255
  int * mayores_list = calloc(mayores +1 , sizeof(int));
  int * menores_list = calloc(menores, sizeof(int));

  int mini= 0;
  int maxi = 0;

  for (int i=0; i < n; i++){
    if (accesos[i] < head){
      menores_list[mini] = accesos[i];
      mini = mini +1;
    }
    else if(accesos[i] >= head){
      mayores_list[maxi] = accesos[i];
      maxi = maxi +1 ;
    }
  }
  mayores_list[mayores] = maximal;
  //ordenamos los accesos
  insertion_sort(mayores +1 , mayores_list);
  insertion_sort(menores, menores_list);
  rvereseArray(menores_list, 0, menores-1);

  // for (int i = 0; i <= mayores; i++){
  //   printf("%d mayores\n", mayores_list[i]);
  // }
  //
  // for (int i = 0; i < menores ; i++){
  //   printf("%d menores\n", menores_list[i]);
  // }

  //juntamos todo en uno
  int index = 0;
  int*temp = calloc(n+1, sizeof(int));
  for (int i =0; i <= mayores; i++){
    temp[index] = mayores_list[i];
    index++;
  }
  for (int i =0; i < menores; i++){
    temp[index] = menores_list[i];
    index++;
  }
  free(mayores_list);
  free(menores_list);

  // for(int i = 0; i <n+1; i++){
  //   printf("loop accesos %d\n", temp[i]);
  // }

  return temp;
}

int*CLOOK(int head, int n, int*accesos, int*tupla){
  int mayores = 0;
  int menores = 0;
  for (int i = 0; i < n; i++){
    if (accesos[i] >= head) mayores = mayores +1;
    else menores = menores + 1;
  }

  //printf("%d %d\n", mayores, menores);
  //le agregamos la ida al 255
  int * mayores_list = calloc(mayores, sizeof(int));
  int * menores_list = calloc(menores, sizeof(int));

  int mini= 0;
  int maxi = 0;

  for (int i=0; i < n; i++){
    if (accesos[i] < head){
      menores_list[mini] = accesos[i];
      mini = mini +1;
    }
    else if(accesos[i] >= head){
      mayores_list[maxi] = accesos[i];
      maxi = maxi +1 ;
    }
  }

  insertion_sort(mayores, mayores_list);
  insertion_sort(menores, menores_list);

  // for (int i = 0; i < mayores; i++){
  //   printf("%d mayores\n", mayores_list[i]);
  // }
  //
  // for (int i = 0; i < menores ; i++){
  //   printf("%d menores\n", menores_list[i]);
  // }

  //juntamos todo en uno
  int index = 0;
  int*temp = calloc(n, sizeof(int));
  for (int i =0; i < mayores; i++){
    temp[index] = mayores_list[i];
    index++;
  }
  for (int i =0; i < menores; i++){
    temp[index] = menores_list[i];
    index++;
  }
  free(mayores_list);
  free(menores_list);

  // for(int i = 0; i <n; i++){
  //   printf("loop accesos %d\n", temp[i]);
  // }

  return temp;


}

int main(int argc, char*argv[]){

  char*politica = argv[1];
  char*input = argv[2];
  int count = 0;


  int head;

  //COntamos cantidad de lineas en el txt
  FILE*fr;
  fr = fopen(input, "r");

  for (char c =getc(fr); c!=EOF; c=getc(fr)){
    if (c == '\n') count++;
  }

  //printf("LINES %d\n", count);

  //Armamos arreglo de ints
  int*accesos = calloc(count - 1, sizeof(int));
  int*tupla = calloc(2, sizeof(int));
  int* new_queue;
  fclose(fr);
  fr = fopen(input, "r");
  fscanf(fr, "%d", &head);
  for (int i= 0; i < count -1; i++){
    fscanf(fr, "%d", &accesos[i]);
  }
  fclose(fr);

  /*
  printf("%d\n", head);
  for (int i =0; i < count-1; i++) printf("%d\n", accesos[i]);
  */

  if (strcmp(politica, "fcfs")==0){
    fcfs(head, count -1,accesos, tupla);
  }

  else if (strcmp(politica, "sstf")==0){
    sstf(head, count-1, accesos, tupla);
    fcfs(head, count-1, accesos, tupla);

  }

  else if (strcmp(politica, "scan")==0){

    new_queue = SCAN(head, count-1, 255, accesos, tupla);
    fcfs(head, count, new_queue, tupla);

  }

  else if (strcmp(politica, "c-look")==0){

    new_queue = CLOOK(head, count-1, accesos, tupla);
    fcfs(head, count-1, new_queue, tupla);

  }

  if(strcmp(politica, "c-look")==0){
    for (int i = 0; i<count-2;i++) printf("%d,",new_queue[i]);
    printf("%d", new_queue[count-2]);
    free(new_queue);


  }

  else if (strcmp(politica, "scan")==0){
    for (int i = 0; i<count-1;i++) printf("%d,",new_queue[i]);
    printf("%d", new_queue[count-1]);
    free(new_queue);


  }

  else {
    for (int i = 0; i<count-2;i++) printf("%d,",accesos[i]);
    printf("%d", accesos[count-2]);

  }

  printf("\n");
  printf("%d", tupla[0]);
  printf("\n");
  printf("%dT + ", tupla[0]);
  printf("%dD msec\n", tupla[1]);

  free(accesos);
  free(tupla);





  return 0;
}
