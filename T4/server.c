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


pthread_cond_t player_to_join;
pthread_mutex_t general_mutex;
int player_is_waiting = 0;
int challenging_player = 0;



void* recieveMessage(void * client_socket){
  int sender = *(int*)client_socket;
  while(1){
    printf("Waiting message... ♔ \n");
    char* message = malloc(sizeof(char)*1024);
    recv(sender, message, 1024, 0);
    printf("%s\n", message);
    sleep(3);
  }

}

void sendMessage(int socket, char* message){
  send(socket, message, 1024,0);
}

//Para refrescar el tablero a los jugadores
void broadcast(wchar_t ** board, char * one_dimension_board, int player_one, int player_two) {

  to_one_dimension_char(board, one_dimension_board);

  printf("\tSending board to %d and %d size(%d)\n", player_one, player_two, sizeof(one_dimension_board));
  send(player_one, one_dimension_board, 64, 0);
  sendMessage(player_two, one_dimension_board);
  printf("\tSent board...\n");
}


void * game_room(void *client_socket){
  int player_one = *(int *)client_socket;
  int n, player_two;
  char buffer[64];
  int * move = (int *)malloc(sizeof(int)*4);

  // Create a new board
  wchar_t ** board = create_board();
  char * one_dimension_board = create_od_board();
  initialize_board(board);

  player_is_waiting = 1; // Set user waiting

  printf("ESPERANDO A PLAYER 2\n");
  pthread_mutex_lock(&general_mutex); // Wait for player two
  printf("CASI CASI\n");
  pthread_cond_wait(&player_to_join, &general_mutex); // Wait for player wants to join signal
  printf("P2 IT IN\n");
  // TODO lock assigning player mutex
  player_two = challenging_player; // Asign the player_two to challenging_player
  player_is_waiting = 0; // Now none is waiting}

  printf("MANDANDO MENSAJES A JUGADORES\n");


  if (send(player_one, "i-p1", 4, 0) < 0) {
     perror("ERROR writing to socket");
     exit(1);
   }
  if (send(player_two, "i-p2", 4, 0) < 0) {
     perror("ERROR writing to socket");
     exit(1);
   }

  sleep(2);
  broadcast(board, one_dimension_board, player_one, player_two);

}




int main (int argc, char*argv[]){
  pthread_t tid[2];

  setlocale(LC_ALL, "en_US.UTF-8");
  int sockfd, client_socket, port_number, client_length;
  char buffer[64];
  struct sockaddr_in server_address, client;
  int n;
  char*ip = IP;

  // Conditional variable
  pthread_cond_init(&player_to_join, NULL);
  pthread_mutex_init(&general_mutex, NULL);


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
  listen(sockfd, 1);
  printf("Server listening on port %d\n", port_number);

  while(1) {
     client_length = sizeof(client);
     // CHECK IF WE'VE A WAITING USER

     /* Accept actual connection from the client */
     client_socket = accept(sockfd, (struct sockaddr *)&client, (unsigned int *)&client_length);
     printf("– Connection accepted from %d at %d.%d.%d.%d:%d –\n", client_socket, client.sin_addr.s_addr&0xFF, (client.sin_addr.s_addr&0xFF00)>>8, (client.sin_addr.s_addr&0xFF0000)>>16, (client.sin_addr.s_addr&0xFF000000)>>24, client.sin_port);

     if (client_socket < 0) {
        perror("ERROR on accept");
        exit(1);
     }


    if (player_is_waiting == 0){
      printf("Connected player listening,\n");
      pthread_create(&tid[0], NULL, &game_room, &client_socket);
      player_is_waiting = 1;


    }
    else {
       // Send user two signal
       printf("Connected player, joining game room... %d\n", client_socket);
       challenging_player = client_socket;
       pthread_mutex_unlock(&general_mutex);
       pthread_cond_signal(&player_to_join);
     }
   }
  return 0;
}
