#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

/*extern enum numbers; { ONE TWO THREE FOUR FIVE SIX SEVEN EIGHT NINE };*/

struct s* reader(FILE *fp, struct s *sp){
  int i,j;
  struct f **sudo_init;
  struct f *tmparray;
  
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      /*int n,ns;
	struct f tmpfield = {n, ns}; */
      printf("Durchlauf: i = %d, j = %d\n",i,j);
      tmparray = malloc(sizeof(struct f));
      /*tmparray[j] = malloc(sizeof(struct f));*/
    }
    printf("Durchlauf aussen: i = %d\n", i);
    sudo_init = &tmparray;
    printf("%s", "sudo_init gesetzt\n");
    if (i == 8) printf("%s", "letzter durchlauf");
    /*    if (i < 8) sudo_init++;*/
  }

  printf("%s", "zugriff auf sp\n");
  struct s *test = malloc(sizeof(struct s));
  test->sudoku_array = sudo_init;
  printf("%s", "Vor der Schleife\n");
  for (i = 0; i < 0; i++){
    for (j = 0; j < 4; j++){
      printf("innen drin: i = %d, j = %d\n", i, j);
      fscanf(fp, "%1d", &(test->sudoku_array[i][j].n));
      test->sudoku_array[i][j].ns = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE;
    }
  }
  return test;
  /* struct s tmpsudoku = {&sudo_init[0]};
     sp = &tmpsudoku;*/

  /*  for (i = 0; i < 9; i++){
    int tmparray[9];
    for (j = 0; j < 9; j++){
      struct f field;
      fscanf(fp, "%1d", &(field.n));
      field.ns = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE;
      tmparray[9] = &field;
    }
    sudoku[i] = tmparray;
    }*/
}
