#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>


/*
RAM
*/

typedef struct RAM {
  char **data;
  int **metadata;
  int count;
  int len;
  int p;
} RAM;

RAM *init_ram(int len, int frames) {
  RAM *new = calloc(1, sizeof(RAM));
  new->data = calloc(len, sizeof(char*));
  new->metadata = calloc(len, sizeof(int*));

  for (int i = 0; i < len; i++) {
    new->data[i] = calloc(frames, sizeof(char));
    new->metadata[i] = calloc(2, sizeof(int));
    new->metadata[i][0] = -1; //page using the frame
    new->metadata[i][1] = 0; //time

  }
  new->len = len;
  new->count = 0;
  new->p = 0;
  return new;
}

void destroy_ram(RAM *ram) {
  for (int i = 0; i < ram->len; i++) {
    free(ram->data[i]);
    free(ram->metadata[i]);
  }
  free(ram->data);
  free(ram->metadata);
  free(ram);
}

int get_next_frame(RAM *ram, int policity, int t) {
  int result;
  if (ram->count < ram->len) {
    result = ram->p;
    ram->p = (ram->p + 1) % ram->len;
    ram->count += 1;
  } else {
    if (policity == 1) {
      result = ram->p;
      ram->p = (ram->p + 1) % ram->len;
    } else {
      int min = ram->metadata[0][1];
      for (int i = 0; i < ram->len; i++) {
        if (ram->metadata[i][1] < min) {
          min = i;
        }
      }
      result = min;
    }
  }
  ram->metadata[result][1] = t;
  return result;
}


/*
  Translation lookaside buffer
*/
typedef struct TLB{
  int **entries;
  int hit;
  int miss;
  int len;
  int p;
  int count;
} TLB;

TLB *new_tlb(int len) {
  TLB *new = calloc(1, sizeof(TLB));
  new->entries = calloc(len, sizeof(int*));
  for (int i = 0; i < len; i++) {
    new->entries[i] = calloc(3, sizeof(int));
    new->entries[i][0] = -1; //not valid data
    new->entries[i][1] = -1; //not valid data
    new->entries[i][2] = 0; // TIME
  }
  new->hit = 0;
  new->miss = 0;
  new->len = len;
  new->count = 0;
  new->p = 0;
  return new;
}

void destroy_tlb(TLB *tlb) {
  for (int i = 0; i < tlb->len; i++) {
    free(tlb->entries[i]);
  }
  free(tlb->entries);
  free(tlb);
}

void insert_in_tlb(TLB *tlb, int index, int key, int value) {
  tlb->entries[index][0] = key;
  tlb->entries[index][1] = value;
}

void updateTLB(TLB *tlb, int page, int frame, int t, int policity) {

  int index;
  if (tlb->count < tlb->len) {
    index = tlb->p;
    tlb->p = (tlb->p + 1) % tlb->len;
    tlb->count += 1;
  } else {
    if (policity == 1) {
      index = tlb->p;
      tlb->p = (tlb->p + 1) % tlb->len;
    } else {
      int current;
      int min = 10000000;
      for (int i = 0; i < tlb->len; i++) {
        if (tlb->entries[i][2] < min) {
          min = tlb->entries[i][2];
          current = i;
        }
      }
      index = current;
    }
  }
  printf("LRU: %d \n", index);
  tlb->entries[index][0] = page;
  tlb->entries[index][1] = frame;
  tlb->entries[index][2] = t;
}

//  Retorna el frame en la pos page o -1 si es que no encuentra la página
int lookupTLB(TLB *tlb, int page, int t) {
  //printf("Buscamos en la TLB la página: %d\n", page);
  for (int i = 0; i < tlb->len; i++) {
    if (tlb->entries[i][0] == page){
      tlb->entries[i][2] = t;
      tlb->hit += 1;
      return tlb->entries[i][1];
    } 
  }
  tlb->miss += 1;
  return -1;
}
/*
  Table Page
*/
typedef struct TP{
  int **entries;
  int hit;
  int miss;
  int len;
  int pointer;
} TP;

TP *new_table_page(int len) {
  TP *new = calloc(1, sizeof(TP));
  new->entries = calloc(len, sizeof(int*));
  for (int i = 0; i < len; i++) {
    new->entries[i] = calloc(3, sizeof(int)); 
    new->entries[i][1] = 0; // VALID BIT; 
    new->entries[i][2] = 0; // TIME;
  }
  new->len = len;
  new->hit = 0;
  new->miss = 0;
  return new;
}

void destroy_tp(TP *tp) {
  for (int i = 0; i < tp->len; i++) {
    free(tp->entries[i]);
  }
  free(tp->entries);
  free(tp);
}

int lookupTP(TP *tp, int page, int t) {
  //printf("Buscamos en la TP la págia: %d\n", page);
  if (tp->entries[page][1] == 0) { // Pagina no valida o en disco
    tp->miss += 1;
    return -1;
  }
  tp->hit += 1;
  tp->entries[page][2] = t;
  return tp->entries[page][0];
}

int get_frame(RAM *ram, TP *tp, int policity, int t) {
  int frame;
  if (ram->count < ram->len) {
    frame = ram->p;
    ram->p = (ram->p + 1) % ram->len;
    ram->count += 1;
  } else {
    if (policity == 1) {
      frame = ram->p;
      ram->p = (ram->p + 1) % ram->len;
    } else {
      int min = 1000000;
      int index = 0;
      for (int i = 0; i < tp->len; i++) {
        if (tp->entries[i][1] == 1) {
          if (tp->entries[i][2] < min) {
            min = tp->entries[i][2];
            index = i;
          }
        }
      }
      frame = tp->entries[index][0];
      tp->entries[index][1] = 0;
      tp->entries[index][2] = 0;
      printf("LRU: %d\n", frame);
    }
  }
  return frame;
}

void updateTP(TP *tp, int page, int frame) {
  tp->entries[page][0] = frame;
  tp->entries[page][1] = 1; // VALID BIT
}


int get_virtual_page_number(int address) {
  uint16_t VPN_MASK = 0xFF00;
  uint16_t VPN_SHIFT = 8;
  return (address & VPN_MASK) >> VPN_SHIFT;
}

int get_offset(int address) {
  uint16_t VPN_MASK = 0x00FF;
  return address & VPN_MASK;
}

void copy_from_disk(RAM* ram, char *filename, int page, int frame, int t, int offset) {
  FILE* fp;
  char buffer[256];
  fp = fopen(filename, "rb");
  fseek(fp, page * 256, SEEK_SET);
  fread(buffer, 1, 256, fp);
  fclose(fp);
  ram->data[frame] = buffer;
}

void touch(TLB *tlb, TP *tp, RAM *ram, int address, int t, int policity) {
  int page = get_virtual_page_number(address);
  int offset = get_offset(address);
  //printf("Address: %d, page: %d, offset: %d\n", address, page, offset);
  
  int frame = lookupTLB(tlb, page, t); //Buscamos en la TLB

  if (frame > -1) {
    // Actualizamos el timpo en la TP
    tp->entries[page][2] = t;
    printf("%d\n", (uint8_t)ram->data[frame][offset]);
  } else { 
    //printf("TLB MISS\n");
    frame = lookupTP(tp, page, t); // Buscamos en la TP
    if (frame > -1) {
      // Tenemos que actualizar la TLB
      updateTLB(tlb, page, frame, t, policity);
      printf("%d\n", (uint8_t)ram->data[frame][offset]);
    } else { 
      // printf("PAGE FAULT\n");
      frame = get_frame(ram, tp, policity, t);
      // frame = get_next_frame(ram, policity, t); //Buscamos un frame libre
      copy_from_disk(ram, "./data.bin", page, frame, t, offset);
      updateTLB(tlb, page, frame, t, policity);
      updateTP(tp, page, frame);
      printf("%d\n", (uint8_t)ram->data[frame][offset]);
    }
  }
}

int main(int argc, char*argv[]) {
  TLB *tlb = new_tlb(32);
  TP *tp = new_table_page(256);
  RAM *ram = init_ram(128, 256);
  int policity; //1: FIFO 2:LRU

  
  if (strcmp(argv[1], "lru") == 0) policity = 2;
  else policity = 1;
  printf("pol: %d\n", policity);
  
  int b;
  int t = 0;
  while(scanf("%d", &b) != EOF) {
    touch(tlb, tp, ram, b, t, policity);
    t += 1;
  }
  printf("PORCENTAJE_PAGE_FAULTS = %d%% \n", tp->miss * 100 / t);
  printf("PORCENTAJE_TLB_HITS = %d%% \n", tlb->hit * 100 / t);
  printf("TABLA DE PAGINAS\n");
  printf("page_number\tframe_number\n");
  for (int i = 0; i < tp->len; i++) {
    printf("%d\t", i);
    if (tp->entries[i][1]) {
      printf("\t%d\n", tp->entries[i][0]);
    } else {
      printf("\n");
    }
  }
  printf("TLB\n");
  printf("i\tpage_number\tframe_number\n");
  for (int i = 0; i < tlb->len; i++) {
    printf("%d\t", i);
    if (tlb->entries[i][0] > -1) {
      printf("%d\t\t%d\n", tlb->entries[i][0], tlb->entries[i][1]);
    } else {
      printf("\n");
    }
  }
  destroy_tlb(tlb);
  //destroy_tp(tp);
  destroy_ram(ram);
  return 0;
}