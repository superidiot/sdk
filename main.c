#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

main (){
  
  int i,j;
  FILE *fp = fopen("sudoku2.txt","r");
  struct s *sp;
  sp = reader(fp);
  fclose(fp);

  
  printer_cli(sp);
  
  /* printer_ps(sp, "sudoku.ps", 1, 11, "w"); */
  /*  solver(sp);*/
  // printer_cli(sp);
  /* printer_ps(sp, "sudoku.ps", 1, 1, "a"); */
}
