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


int main (int argc, char * argv[]){
  wchar_t ** board = create_board();
  char * one_dimension_board = create_od_board();
  initialize_board(board);
  to_one_dimension_char(board, one_dimension_board);
  print_board_buff(one_dimension_board);
  return 0;
}
