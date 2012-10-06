#include <stdio.h>
#include "sudoku.h"

main (){
  struct s *sp = &sudoku;
 
  FILE *fp = fopen("sudoku2.txt","r");
  reader(fp,sp);
  fclose(fp);
  printer_cli(sp);
  printer_ps(sp, "sudoku.ps", 1, 11, "w");
  solver(sp);
  printer_cli(sp);
  printer_ps(sp, "sudoku.ps", 1, 1, "a");
}
