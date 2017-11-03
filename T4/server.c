#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <signal.h>

#include "board.c"

#define PORT 8080;
#define IP "0.0.0.0"

//Estrcutura para argumentos listeningstruc
struct arg_listening{
  int client_socket;
  char ** UserArray;
};


int mod(int a, int b){
  if (b < 0) return mod(a, -b);
  int ret =  a % b;
  if (ret < 0) ret += b;
  return ret;
}

void Move(char*move, wchar_t**board){
  //colI, filI, colF, filF, identificador 1 y 2 ID y tamaño
  board[move[5]][move[4]] = board[move[3]][move[2]];
  board[move[3]][move[2]] = 0;

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
  char id = 13;
  char tam = contador_piezas;
  arreglo_game[0] = id;
  arreglo_game[1] = tam;
  //strncat(arreglo_game, &id, 1);
  //strncat(arreglo_game, &tam,1);
  int indice = 2;
  for (int i = 0; i < 64; i++){
    char tipo;
    if (one_dimension_board[i] == '1') tipo = 3;
    else if (one_dimension_board[i] == '2') tipo = 5;
    else if (one_dimension_board[i] == '3') tipo = 4;
    else if (one_dimension_board[i] == '4') tipo = 2;
    else if (one_dimension_board[i] == '5') tipo = 1;
    else if (one_dimension_board[i] == '6') tipo = 6;
    else if (one_dimension_board[i] == '7') tipo = 3;
    else if (one_dimension_board[i] == '8') tipo = 5;
    else if (one_dimension_board[i] == '9') tipo = 4;
    else if (one_dimension_board[i] == 'a') tipo = 2;
    else if (one_dimension_board[i] == 'b') tipo = 1;
    else if (one_dimension_board[i] == 'c') tipo = 6;

    char fila = fila_pos;
    char col = mod(i, 8);
    // SI es blanca 0
    if(one_dimension_board[i] == '1' || one_dimension_board[i] == '2' || one_dimension_board[i] == '3'
  || one_dimension_board[i] == '4'|| one_dimension_board[i] == '5'|| one_dimension_board[i] == '6') {

    char color = 0;
    arreglo_game[indice] = color;
    arreglo_game[indice+1] = col;
    arreglo_game[indice +2] = fila;
    arreglo_game[indice+3] = tipo;
     if (mod(i,8) ==7) fila_pos ++;

     printf("PRINTEAMOS SECUENCIA %d ,%d, %d, %d \n", color,col,fila, tipo);
     indice = indice + 4;


    }
    // SI es negra 1
    else if(one_dimension_board[i] == '7' || one_dimension_board[i] == '8' || one_dimension_board[i] == '9'
  || one_dimension_board[i] == 'a'|| one_dimension_board[i] == 'b'|| one_dimension_board[i] == 'c'){
    char color = 1;
    arreglo_game[indice] = color;
    arreglo_game[indice+1] = col;
    arreglo_game[indice +2] = fila;
    arreglo_game[indice+3] = tipo;

     if (mod(i,8) ==7) fila_pos ++;
     printf("PRINTEAMOS SECUENCIA %d ,%d, %d, %d \n", color,col,fila, tipo);
     indice = indice + 4;

    }

  }
  return arreglo_game;
}

int compareStrings(char *s1, char *s2) {
  return !strcmp(s1, s2);
}


pthread_cond_t player_to_join;
pthread_mutex_t general_mutex;
pthread_cond_t respond_game_start;
pthread_mutex_t listening_mutex;

int player_is_waiting = 0;
int challenging_player = 0;
int respondiendo_game_start = 0;
/* CURRENT USERS */

char ** createUserArray() {
  char **users = calloc(1024, sizeof(char*));
  for (int i = 0; i < 1024; i++) {
    users[i] = calloc(1, 256*sizeof(char));
  }
  return users;
}

int loggedUsers = 0;

void sendMessage(int socket, char* message){
  send(socket, message, 1024,0);
}




u_int16_t getId(char * username, char ** userArray) {
  printf("Buscamos el ususario %s \n", username);
  for (int i = 0; i < loggedUsers; i++) {
    if (compareStrings(userArray[i], username)) {
      return (u_int16_t)i;
    }
  }
  strcpy(userArray[loggedUsers], username);
  loggedUsers += 1;
  printf("Asiganmos un nievo id: %d \n", loggedUsers);
  return (u_int16_t)loggedUsers;
}

void responseMatchmaking(int sock, char * username, char ** userArray) {
  u_int16_t id = getId(username, userArray);
  printf("id: %d", id);
}

void BoardAuthority(char * one_dimension_board, int cliente){
  char * arreglo_game = gamesyncboard(one_dimension_board);
  send(cliente, arreglo_game, 1024, 0);


}

//Para refrescar el tablero a los jugadores
void broadcast(wchar_t ** board, char * one_dimension_board, int player_one, int player_two) {

  to_one_dimension_char(board, one_dimension_board);

  printf("\tSending board to %d and %d size(%d)\n", player_one, player_two, sizeof(one_dimension_board));
  BoardAuthority(one_dimension_board, player_one);
  BoardAuthority(one_dimension_board, player_two);

  printf("\tSent board...\n");

}

////Game STart
//7-tamaño(3) - ID-color
void GameStart(int sock, int color, int id){
  printf("Enviando info a usuario\n");
  char * string = malloc(sizeof(char)*256);
  char id_char = 7;
  char tam = 3;
  strncat(string, &id_char, 1);
  strncat(string, &tam, 1);

  char id_p = id;
  strncat(string, &id_p, 1);
  char col = color;
  strncat(string, &col, 1);

  printf("Enviando ID %d tamaño %d\n", string[0], string[1]);
  send(sock, string, 1024, 0);
}


void * game_room(void *client_socket){
  int player_one = *(int *)client_socket;
  int player_two;

  int * move = (int *)malloc(sizeof(int)*4);

  // Create a new board
  wchar_t ** board = create_board();
  char * one_dimension_board = create_od_board();
  initialize_board(board);

  player_is_waiting = 1; // Set user waiting
  printf("ESPERANDO A PLAYER 2\n");
  pthread_mutex_lock(&general_mutex); // Wait for player two

  pthread_cond_wait(&player_to_join, &general_mutex); // Wait for player wants to join signal



  // TODO lock assigning player mutex
  player_two = challenging_player; // Asign the player_two to challenging_player
  player_is_waiting = 0; // Now none is waiting}


  //Enviamos Game STart 0 blancas 1 negras
  GameStart(player_one, 0, player_one);
  GameStart(player_two, 1, player_two);


  //Una vez que ambos respondan rehabilitamos el recibir mensajes de listenin



  broadcast(board, one_dimension_board, player_one, player_two);

  sleep(1);
  send(player_one, "Your Turn\n", 1024, 0);
  send(player_two, "Waiting for Oponent\n", 1024, 0);

  pthread_cond_signal(&respond_game_start);
  //pthread_cond_signal(&respond_game_start);

}

void *TodosListen(void * client_socket){
  int client = *(int *)client_socket;
  pthread_t t[2];
  int jugando = 0;
  printf("Escuachando a %d \n", client);
  while(1) {
    char buffer[1024];
    // recieveMessage(client, buffer);
    printf("Esperando a %d\n", client);
    recv(client, buffer, 1024, 0);
    printf("PRINTEAMOS BUFFER %s\n", buffer);
    char opcion = buffer[0];
    if (jugando == 1){

    }

    else{
      if (opcion == '2') {
        printf("Response match\n");
        //responseMatchmaking(client, &buffer[2], userArray);
       }

       //Gamestart
       if (opcion == '4'){
         printf("Buscando partida\n");
         if (player_is_waiting == 0){
           printf("Connected player listening,\n");
           pthread_create(&t[0], NULL, &game_room, &client);
           player_is_waiting = 1;
           //Respuesta a GameStart
           recv(client, buffer, 1024, 0);
           jugando = 1;
           

         }
         else {
            // Send user two signal
            printf("Connected player, joining game room... %d\n", client);
            challenging_player = client;
            pthread_cond_signal(&player_to_join);
            //Respuesta a GameStart
            recv(client, buffer, 1024, 0);
            jugando = 1;
          }

       }

       if(opcion == '3'){
         //Test
         printf("OPCION TEST\n");
         GameStart(client, 0, 100);
       }
    }


  }
}

void *generalListen(void * arguments) {
  struct arg_listening *args = arguments;
  pthread_t t[2];
  int client = args->client_socket;
  char ** userArray = args->UserArray;

  printf("Escuachando a %d \n", client);
  while(1) {
    char buffer[1024];
    // recieveMessage(client, buffer);
    printf("Esperando a %d\n", client);
    recv(client, buffer, 1024, 0);
    printf("PRINTEAMOS BUFFER %s\n", buffer);
    char opcion = buffer[0];


    if (opcion == '2') {
      printf("Response match\n");
       responseMatchmaking(client, &buffer[2], userArray);
     }

     if (opcion == '4'){
       printf("Buscando partida\n");
       if (player_is_waiting == 0){
         printf("Connected player listening,\n");
         pthread_create(&t[0], NULL, &game_room, &client);
         player_is_waiting = 1;
         pthread_mutex_lock(&listening_mutex);
         pthread_cond_wait(&respond_game_start, &listening_mutex);

       }
       else {
          // Send user two signal
          printf("Connected player, joining game room... %d\n", client);
          challenging_player = client;
          pthread_cond_signal(&player_to_join);

          pthread_mutex_lock(&listening_mutex);
          pthread_cond_wait(&respond_game_start, &listening_mutex);
          printf("CONTINUO");
        }
     }

     if(opcion == '3'){
       //Test
       printf("OPCION TEST\n");
       GameStart(client, 0, 100);
     }
    sleep(1);
  }
}






int main (int argc, char*argv[]){
  pthread_t tid[2];

  setlocale(LC_ALL, "en_US.UTF-8");
  int sockfd, client_socket, port_number, client_length;

  struct sockaddr_in server_address, client;
  char*ip = IP;

  // Conditional variable
  pthread_cond_init(&player_to_join, NULL);
  pthread_mutex_init(&general_mutex, NULL);
  pthread_mutex_init(&listening_mutex, NULL);
  pthread_cond_init(&respond_game_start, NULL);


  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &server_address, sizeof(server_address));
  port_number = PORT;

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(ip);
  server_address.sin_port = htons(port_number);

  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
      perror("ERROR on binding");
      exit(1);
  }


  /* MAX_QUEUE */
  listen(sockfd, 5);
  printf("Server listening on port %d\n", port_number);

  /* LISTENING */

  char ** userArray = createUserArray();
  struct arg_listening list_args;
  list_args.UserArray = userArray;



  while(1) {
     client_length = sizeof(client);

     // CHECK IF WE'VE A WAITING USER

     /* Accept actual connection from the client */
     printf("ESPERANDO ACCEPT \n");
     client_socket = accept(sockfd, (struct sockaddr *)&client, (unsigned int *)&client_length);

     //Lenamos la struct de listening
     list_args.client_socket = client_socket;

     //pthread_create(&tid[1], NULL, &generalListen, &list_args);
     pthread_create(&tid[1], NULL, &TodosListen, &client_socket);
     printf("– Connection accepted from %d at %d.%d.%d.%d:%d –\n", client_socket, client.sin_addr.s_addr&0xFF, (client.sin_addr.s_addr&0xFF00)>>8, (client.sin_addr.s_addr&0xFF0000)>>16, (client.sin_addr.s_addr&0xFF000000)>>24, client.sin_port);

     if (client_socket < 0) {
        perror("ERROR on accept");
        exit(1);
     }



     sleep(1);
   }
  return 0;
}
