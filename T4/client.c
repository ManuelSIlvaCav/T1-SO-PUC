#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "board.c"


void* recieveMessage(void * client_socket){
  int sender = *(int*)client_socket;
  int n;

  while(1){
    printf("Waiting message... ♔ \n");
    char* buffer = malloc(sizeof(char)*1024);
    recv(sender, buffer, 1024, 0);
    if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == '\0')
    {
      printf("Mensaje %s", buffer);
    }
    else {
      print_board_buff(buffer);
    }

    sleep(3);

  }

}

void * requestMatchMaking(int sock, char * username) {
  int len = strlen(username);
  char c = len + '0';

  char * string = malloc(sizeof(char)*256);
  strcpy(string, "2");
  //printf("Printeamos por separada %s\n", string);
  strncat(string, &c, 1);
  //printf("Printeamos por separada %s\n", string);
  strcat(string, username);
  printf("ENVIANDO %s\n", string);
  int success = send(sock, string, 1024, 0);
  printf("MEnsaje enviado :%d\n", success);
}


int compareStrings(char *s1, char *s2) {
  return !strcmp(s1, s2);
}

int main(int argc, char *argv[]){
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent * server;
  pthread_t tid[1];

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
    // pthread_create(&tid[0], NULL, &recieveMessage, &sockfd);

    while (1) {
      char* selection = malloc(sizeof(char));
      printf("\n");
      printf("Seleciona una opción\n");
      printf("[2]: Pedir matchmaking\n");
      scanf("%s", selection);
      if (compareStrings(selection, "2")) {
        char * username = calloc(256, sizeof(char));
        printf("Elige un username\n");
        scanf("%s", username);
        requestMatchMaking(sockfd, username);
      }

    }
    return 0;

}
