#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define LSH_TOK_DELIM " \t\r\n\a"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}


char *msh_read_line(void){
  char*line = NULL;
  size_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  //printf("LINE %s", line);
  return line;
}


char **msh_split_line(char * line){

  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));

  char *token;

  if (!tokens){
    fprintf(stderr, "msh alloc error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while(token != NULL){
    tokens[position] = token;
    position++;

    if (position >= bufsize){
      bufsize +=  64;
      tokens = realloc(tokens, bufsize * sizeof(char*));

      if (!tokens){
        fprintf(stderr, "MSH ALLOC ERROR\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);

  }

  tokens[position] = NULL;
  return tokens;
}



int msh_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0){
    //child
    if (execvp(args[0], args)== -1){
      perror("MSH");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid < 0) {
      // Error forking
      perror("MSH");
    }
  else {
      // Parent process
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;

}


int msh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  return msh_launch(args);
}





void msh_loop(void){
  char *line;
  char **args;
  int status;

  do {
    printf("Primer Comando > ");
    line = msh_read_line();
    args = msh_split_line(line);
    status = msh_execute(args);

    free(line);
    //free(args);
  } while(status);
}

int main(int argc, char* argv[]) {

  msh_loop();


  return EXIT_SUCCESS;
}
