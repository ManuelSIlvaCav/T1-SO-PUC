#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "board.c"

pthread_mutex_t general_mutex;
pthread_cond_t respond_game;

int jugando= 0;
int turno = 0;

int mod(int a, int b){
  if (b < 0) return mod(a, -b);
  int ret =  a % b;
  if (ret < 0) ret += b;
  return ret;
}



char*gamesyncboard(char*one_dimension_board){
  //Solo me interesan la posicion de las piezas
  int contador_piezas = 0;
  for (int i = 0; i < 64; i++){
    if (one_dimension_board[i] == '1') contador_piezas++;
    else if (one_dimension_board[i] == '2') contador_piezas++;
    else if (one_dimension_board[i] == '3') contador_piezas++;
    else if (one_dimension_board[i] == '4') contador_piezas++;
    else if (one_dimension_board[i] == '5') contador_piezas++;
    else if (one_dimension_board[i] == '6') contador_piezas++;
    else if (one_dimension_board[i] == '7') contador_piezas++;
    else if (one_dimension_board[i] == '8') contador_piezas++;
    else if (one_dimension_board[i] == '9') contador_piezas++;
    else if (one_dimension_board[i] == 'a') contador_piezas++;
    else if (one_dimension_board[i] == 'b') contador_piezas++;
    else if (one_dimension_board[i] == 'c') contador_piezas++;
  }

  char* arreglo_game = (char *) malloc(sizeof(char *)*(contador_piezas*4 + 2));
  int fila_pos = 0;
  char id = 12;
  char tam = contador_piezas;
  strncat(arreglo_game, &id, 1);
  strncat(arreglo_game, &tam,1);

  for (int i = 0; i < 64; i++){
    char*pos = malloc(sizeof(char)*4);
    char fila = fila_pos;
    char col = mod(i, 8);
    // SI es blanca 0
    if(one_dimension_board[i] == '1' || one_dimension_board[i] == '2' || one_dimension_board[i] == '3'
  || one_dimension_board[i] == '4'|| one_dimension_board[i] == '5'|| one_dimension_board[i] == '6') {
     char tipo = 0;
     strncat(pos, &tipo, 1);
     strncat(pos, &col, 1);
     strncat(pos, &fila, 1);
     if (mod(i,8) ==7) fila_pos ++;
     //printf("PRINTEAMOS SECUENCIA %s ,%c,%c \n", pos,fila,col);
     strcat(arreglo_game, pos);

    }
    // SI es negra 1
    if(one_dimension_board[i] == '7' || one_dimension_board[i] == '8' || one_dimension_board[i] == '9'
  || one_dimension_board[i] == 'a'|| one_dimension_board[i] == 'b'|| one_dimension_board[i] == 'c'){
    char tipo = 1;
     strncat(pos,&tipo, 1);
     strncat(pos, &col, 1);
     strncat(pos, &fila, 1);
     if (mod(i,8) ==7) fila_pos ++;
     //printf("PRINTEAMOS SECUENCIA %s ,%c,%c \n", pos,fila,col);
     strcat(arreglo_game, pos);
    }

  }
  //printf("Secuencia FInal %s", arreglo_game);
  printf("ID: %d Largo %d \nSec %s\n", arreglo_game[0], arreglo_game[1], &arreglo_game[2]);
  return arreglo_game;
}

void GameSync(char * one_dimension_board, int sock){

  char * arreglo_game = gamesyncboard(one_dimension_board);

  //send(sock, arreglo_game, 1024, 0);

}

void SyncToBoard(char * one_dimension_board, char*arreglo_game, wchar_t**board){
  //Funcion que me lleva desde el paquete codificado en bytes a un arreglo de strings one_dim_board
  int porte = arreglo_game[1];

  for (int i = 2; i < porte*4; i+=4){

  }

}

void* recieveMessage(void * client_socket){
  int sender = *(int*)client_socket;
  wchar_t ** board = create_board();
  char * one_dimension_board = create_od_board();
  initialize_board(board);

  while(1){
    printf("Waiting message... ♔ \n");
    char* buffer = malloc(sizeof(char)*1024);
    recv(sender, buffer, 1024, 0);
    if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == '\0')
    {
      printf("Mensaje %s", buffer);
    }

    else if (buffer[0] == 7){
      //CASo de GameStart;
      char color = buffer[3];
      char * string = malloc(sizeof(char)*3);
      strncat(string, &buffer[2],1 );
      strncat(string, &color, 1);
      printf("MENSAJE Respuesta Gamereq %d, %d\n", string[0], string[1]);
      send(sender, string, 1024, 0);
      jugando = 1;
      if (color == 0 ) turno = 1;
      printf("Jugando %d, Turno %d", jugando, turno);

    }
    else if(buffer[0] == 13){
      printf("Enviandome BOARDAUTHORITY\n");
      //Recibo el arreglo de piezas ID13Tamano(color,col,fila,pieza)
      SyncToBoard(one_dimension_board, buffer, board);


    }

    else if (buffer[0] == 16){
      printf("REIBO GAMEREQUEST ENVO GAMSYNC");
    }
    else {

      printf("PRINTEAMOS Mensaje General %s", buffer);
    }

    sleep(1);

  }

}


void * requestMatchMaking(int sock, char * username) {
  int len = strlen(username);
  char id = 2;
  char c = len;
  char * string = malloc(sizeof(char)*(3+len));
  string[0] = id;
  string[1] = c;
  string[2] = '\0';
  strcat(string, username);
  printf("Probamos el Print id: %d, len: %d, username: %s", string[0], string[1], &string[2]);
  int success = send(sock, string, 1024, 0);
  printf("MEnsaje enviado :%d\n", success);
}


int compareStrings(char *s1, char *s2) {
  return !strcmp(s1, s2);
}

int main(int argc, char *argv[]){
  pthread_t tid[2];
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent * server;


  pthread_mutex_init(&general_mutex, NULL);
  pthread_cond_init(&respond_game, NULL);

  setlocale(LC_ALL, "en_US.UTF-8");
  char buffer[64];

  if (argv[2] == NULL) {
     portno = 80;
   } else {
     portno = atoi(argv[2]);
   }


   printf("Connecting to %s:%d\n", argv[1], portno);

   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   server = gethostbyname(argv[1]);

   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
    }

    // printf("Connected player listening,\n");
    pthread_create(&tid[0], NULL, &recieveMessage, &sockfd);

    while (1) {
      if (jugando){
        printf("ENTRANDO A JUGAR \n");
        if (turno){
          char*jugada = malloc(sizeof(char)*5);
          printf("Ingrese Jugada for Col inicial, fila inicial, col final, fila final");
          scanf("%s", jugada);
        }
        else{
          printf("Espere a su Oponente");
          sleep(4);
        }
      }
      else{
        char* selection = malloc(sizeof(char));
        printf("\n");
        printf("Seleciona una opción\n");
        printf("[2]: Pedir matchmaking\n [4]:BUscar Partida\n");
        scanf("%s", selection);
        if (compareStrings(selection, "2")) {
          char * username = malloc(sizeof(char)*256);
          printf("Elige un username\n");
          scanf("%s", username);
          requestMatchMaking(sockfd, username);
        }

        else if (compareStrings(selection, "4")){
          send(sockfd, "4", 1024, 0);
        }

        else if (compareStrings(selection, "3")){
          send(sockfd, "3", 1024, 0);
        }

        else {
          send(sockfd, selection, 1024, 0);
        }

      }
      sleep(1);
    }
    return 0;

}
