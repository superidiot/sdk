#include <stdio.h>
#include "sudoku.h"

main (){
  struct s *sp = &sudoku;
 
  FILE *fp = fopen("sudoku2.txt","r");
  reader(fp,sp);
  fclose(fp);
  printer(sp);
  solver(sp);
  printer(sp);
}
