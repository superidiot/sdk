#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

main (){
  
  int i,j;
  struct f **sarray;
  FILE *fp = fopen("sudoku2.txt","r");
  for (i = 0; i < 9; i++){
    struct f *farray;
    for (j = 0; j < 9; j++){
      struct f *test;
      test = (struct f *) malloc(sizeof(struct f));
      fscanf(fp, "%1d", &(test->n));
      test->ns = test->ns | ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE;
      farray = test;
      farray++;
    }
    sarray = (struct f **) malloc(sizeof(struct f *));
    sarray = &farray;
    sarray++;
    printf("test: %d\n", farray[i].ns);
  }
  fclose(fp);

  /*
    struct s *sp;
    FILE *fp = fopen("sudoku2.txt","r");
    sp = reader(fp,sp);
    fclose(fp);
    
    printer_cli(sp); 
  */
  /*printer_ps(sp, "sudoku.ps", 1, 11, "w");
  solver(sp);
  printer_cli(sp);
  printer_ps(sp, "sudoku.ps", 1, 1, "a"); */
}
