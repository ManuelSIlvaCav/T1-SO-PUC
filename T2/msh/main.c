#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



#define LSH_TOK_DELIM " \t\r\n\a"


static volatile int keepRunning = 1;
static volatile int MainLoop = 1;



void intHandler(int dummy) {
    keepRunning = 0;
}


void intHandler2 (int dummy){
  printf("\nADIOS SALUDOS!\n");
  exit(EXIT_SUCCESS);
}

//BUILTINS
int msh_exit(char **args);
char* set_prompt(char** args, char*line, int status);
char* newpath(char**args, char*path);
char* newarg0(char*path, char*arg0);
char* revalidate_path(char*path_info, char*string);


int countdigits(int n){
  int count = 0;
  while(n!=0){
    n /= 10;
    count ++;
  }
  return count;
}
char *builtin_str[] = {
  "exit",
};

int (*builtin_func[]) (char **) = {
  &msh_exit,
};

int msh_exit(char **args)
{
  //printf("ENTRANDO EXIT\n ");
  MainLoop = 0;
}

char* set_prompt(char** args, char*prompt_message, int status){
  int len = strlen(args[1]);
  prompt_message = malloc((len+1)*sizeof(char));
  strcpy(prompt_message, args[1]);
  //Buscamos el *
  char*ptr;
  int index;
  ptr = strstr(args[1], "*");
  //SI existe el * en el string
  //printf("STATUS %d", status);
  if (ptr != NULL){

    index = (int)(ptr - args[1]);
    char *new_string = malloc((len + 3)*sizeof(char));


    int n = countdigits(status);
    //int to str
    char *num_str = malloc((n+1)*sizeof(char));
    snprintf(num_str, n+1, "%d", status);

    //printf("NEW STR %s and count %d\n", num_str, n);

    //printf("INDEX %d LEN %d\n", index, len);

    if (index == len -1){

       memcpy(new_string, prompt_message, len-1);
       memcpy(&new_string[len-1], num_str, n);
       printf("%s\n", new_string);

     }
    else if (index == 0){

      memcpy(new_string, num_str, n);
      memcpy(&new_string[n], &prompt_message[1], len-1);
      printf("%s\n", new_string);

    }
    else {
      memcpy(new_string, prompt_message, index);
      memcpy(&new_string[index], num_str, n);
      memcpy(&new_string[index + n], &prompt_message[index+1], len - index);
      printf("%s\n", new_string);
    }

    return new_string;
  }



  //printf("Seteandola %s \n", prompt_message);
  return prompt_message;
}

char*newpath(char**args, char*path_info){
  int len = strlen(args[1]);
  path_info = malloc(len*sizeof(char));
  strcpy(path_info, args[1]);
  return path_info;
}

char* newarg0(char*path_info, char*arg0){
  //printf("OLD PATH %s\n", path_info);
  int len1 = strlen(arg0);
  int len2 = strlen(path_info);
  path_info[len2] = '\0';
  //printf("NEW PATH CHANGE %s\n", path_info);
  char*newstring = malloc((len1 + len2)*sizeof(char));
  strcat(newstring, path_info);
  strcat(newstring, arg0);
  //printf("LAST PATH COM %s\n", newstring);
  return newstring;

}

char* revalidate_path(char*path_info, char*string){
  //printf("OLD PATH %s\n", path_info);
  int len1 = strlen(path_info);
  int len2 = strlen(string);
  char*newstring = malloc((len1 + len2)*sizeof(char));
  strcat(newstring, path_info);
  strcat(newstring, string);
  //printf("LAST PATH REVALIDADO %s\n", newstring);
  return newstring;
}

int FindLen(char**args){
  int count = 0;
  for (int i = 0; args[i]; i++){
    //printf("ARG %i: %s\n", i, args[i]);
    count++;
  }
  return count;
}

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
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
      signal(SIGINT, intHandler);
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
        //printf("PROCESO TERMINADO???\n");
        if (keepRunning == 0){
          kill(pid, SIGKILL);
        }
      } while (!WIFEXITED(status) && !WIFSIGNALED(status) && keepRunning);
      //printf("PROCESO BACK ME SALI DEL LOOP??? %d %d\n", wpid, status);
    }

    return status;

}

int msh_launch_back(char**args){
  //printf("EJECUTANDO EN BACK\n");
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
        wpid = waitpid(pid, &status, WNOHANG);

      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
      //printf("PROCESO BACK ME SALI DEL LOOP??? %d\n", wpid);
    }

    return status;
}

int msh_launch_multiple(char**args, int cantidad_procesos)
{

  pid_t pid, wpid;
  int status;
  int forks[cantidad_procesos];

  for(int i =0; i < cantidad_procesos; i++){
    pid = fork();
    if (pid==0){
      if (execvp(args[0],args)==-1){
        perror("MSH MULTIPLE");
      }
      exit(EXIT_FAILURE);
    }
    else if (pid < 0){
      perror("MSH MULTIPLE");
    }

    else{

      forks[i] = pid;
    }

  }
  signal(SIGINT, intHandler);
  for (int i =0; i < cantidad_procesos; i++){
    do {
      wpid = waitpid(forks[i], &status, WUNTRACED);
      //printf("PROCESO TERMINADO %d %d???\n", forks[i], status);
      if (keepRunning == 0){
        kill(pid, SIGKILL);
      }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

  }

  return status;
}



int msh_execute(char **args, int condition, int num_procesos)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  //Si no se definio un setPath
  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  if (condition == 0) return msh_launch(args);
  else if (condition == 1) return msh_launch_back(args);
  else if (condition == 2) return msh_launch_multiple(args, num_procesos);

}




void msh_loop(void){
  char *line;
  char **args;
  int status = 1;
  char* prompt_message = malloc(256);
  prompt_message = "Primera linea";
  int con_path = 0;
  char *path_info = malloc(256);


  do {

    signal(SIGINT, intHandler2);
    printf("%s> ", prompt_message);


    line = msh_read_line();

    args = msh_split_line(line);


    //Auxiliar para saber el final del arreglo de inputs
    int len_arg = FindLen(args);
    //printf("LArgo de args %d\n", len_arg);
    //SI establecimos un path modificamos el arg[0] para usarlo


    //SetPrompt
    if (strcmp(args[0], "setPrompt")==0){
      prompt_message = set_prompt(args, prompt_message, status);

    }

    //setPath
    else if (strcmp(args[0], "setPath")==0){
      path_info = newpath(args, path_info);
      con_path = 1;
      //printf("NEW PATH MSH LOOP %s\n", path_info);
    }

    else if (strcmp(args[0], "resetPath")==0){
      con_path = 0;
    }


    //Todo lo demas &, &N, con path
    else {
      if (con_path==1){
        args[0] = newarg0(path_info, args[0]);
        //printf("INSTRUCCION CON PATH %s\n", args[0]);
      }

      char *parte_uno;
      int cantidad_procesos;
      char *tokens;


      if (strchr(args[len_arg-1], '&')!=NULL){
        //printf("Exist el caracter & en el string\n" );

        //Proceso Background
        if (strcmp(args[len_arg-1], "&")==0 ){
          //printf("PRCESO BACKGROUND!!!\n");
          //Eliminamos el ultimo comando para el execute
          args[len_arg-1] = NULL;
          status = msh_execute(args, 1, 0);
        }
        //De lo contrario son N procesos
        else{
          //EL numero de procesos despues de &
          tokens = strtok(args[len_arg-1], "&");
          cantidad_procesos = strtol(tokens, &parte_uno, 10);

          //printf("PRUEBA PRINTF & %d\n", cantidad_procesos);
          args[len_arg-1] = NULL;
          status = msh_execute(args, 2, cantidad_procesos);

        }

      }

      //Ejecutamos
      else {
        status = msh_execute(args, 0, 0);
      }

      //Revalidamos el path
      char*string = ".";
      path_info = revalidate_path(path_info, string);
    }

    free(line);
    free(args);
    //free(args);
  } while(MainLoop==1);

  printf("ADIOS SALUDOS!\n");
}

int main(int argc, char* argv[]) {
  msh_loop();


  return EXIT_SUCCESS;
}
