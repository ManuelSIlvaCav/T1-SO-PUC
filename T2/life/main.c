#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/*
https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
*/

int** init_matrix(int r, int c) {
  int **new = calloc(r, sizeof(int*));
  for (int i = 0; i < r; i++) {
    new[i] = calloc(c, sizeof(int));
  }
  return new;
}

void destroy_matrix(int **matrix, int r) {
  for (int i = 0; i < r; i ++) {
    free(matrix[i]);
  }
  free(matrix);
}

void copy_matrix(int **source, int **dest, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      dest[i][j] = source[i][j];
    }
  }
}

void print_matrix(int **matrix, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int there_is_life(int **matrix, int row, int col, int i, int j) {
  int counter = -matrix[i][j];
  for (int k = -1; k < 2; k++ ) {
    for (int l = -1; l < 2; l++) {
      if (i + k >= 0 && i + k < row && j + l >= 0 && j + l < col) {
        counter += matrix[i + k][j + l];
      }
    }
  }
  if (counter < 2) return 0;
  if (counter > 3) return 0;
  if (counter == 3) return 1;
  return matrix[i][j];
}

int** life(int **state, int row, int col) {
  int **next_state = init_matrix(row, col);
  copy_matrix(state, next_state, row, col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      next_state[i][j] = there_is_life(state, row, col, i, j);
    }
  }
  return next_state;

}

int **create_matrix(int r, int c) {
  int **data = calloc(r, sizeof(int*));
  for (int i = 0; i < r; i++) {
    data[i] = calloc(c, sizeof(int));
  }
  return data;
}

void *create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  return mmap(NULL, size, protection, visibility, 0, 0);
}

int **create_shared_state(int row, int col) {
  int **state;
  state = (int**) create_shared_memory((size_t)(row * sizeof(int*)));
  for (int i = 0; i < row; i++) {
    state[i] = (int*) create_shared_memory((size_t)(col * sizeof(int)));
  }
  return state;
}

int* get_shared_time() {
  int *time = create_shared_memory(sizeof(int));
  return time;
}


int main(int argc, char *argv[]) {

  char input[20];
  char *tc, *fc, *cc, *vc, *nc;
  fgets(input, 20, stdin);

  tc = strtok(input, " ");
  fc = strtok(NULL, " ");
  cc = strtok(NULL, " ");
  vc = strtok(NULL, " ");
  nc = strtok(NULL, " ");

  int t_max = atoi(tc);
  int row = atoi(fc);
  int col = atoi(cc);
  int v = atoi(vc);
  int n = atoi(nc);


  int **matrix = init_matrix(row, col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrix[i][j] = 0;
    }
  }

  for (int cel = 0; cel < v; cel++) {
    char line[20];
    char *ic, *jc;
    fgets(line, sizeof(line), stdin);
    ic = strtok(line, " ");
    jc = strtok(NULL, " ");
    int i = atoi(ic);
    int j = atoi(jc);
    matrix[i][j] = 1;
  }


  int** shared_state = create_shared_state(row, col);
  copy_matrix(matrix, shared_state, row, col);



  int core_number = sysconf(_SC_NPROCESSORS_ONLN);
  /*
  pid_t pids[core_number];
  for (int i = 0; i < core_number; i++) {
    pids[i] = fork();
    if (pids[i] == 0) {
      sleep(i);
      printf("Entramos");
      int **local_state;
      copy_matrix(shared_state, local_state, row, col);
      for (int t = 0; t < t_max; t++) {
        local_state = life(local_state, row, col);
      }
      print_matrix(local_state, row, col);
      //memcpy(shmem, &str, sizeof(str));
      exit(0);
    }
  }
  */

  /*
  for (int i = 0; i < core_number; i++) {
    wait(NULL);
  }
  */
  int **state;
  for (int t = 0; t < t_max; t++) {
    printf("Iteración %d", t);
    state = life(matrix, row, col);
  }
  print_matrix(state, row, col);


  printf("Adiós\n");
  return 0;
}