#include <stdio.h>
#include "sudoku.h"

main (){

  struct s *sp = &sudoku;
 
  FILE *fp = fopen("sudoku.txt","r");
  reader(fp,sp);
  fclose(fp);
  printer(sp);
}
