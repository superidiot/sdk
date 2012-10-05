#include <stdio.h>
#include "sudoku.h"

main (){
  struct s *sp = &sudoku;
 
  FILE *fp = fopen("sudoku2.txt","r");
  reader(fp,sp);
  fclose(fp);
  printer_cli(sp);
  printer_ps(sp, "in_sudoku.ps");
  solver(sp);
  printer_cli(sp);
  printer_ps(sp, "out_sudoku.ps");
}
