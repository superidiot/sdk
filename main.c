#include <stdio.h>
#include "sudoku.h"

main (){
  FILE *fp = fopen("sudoku.txt","r");
  reader(fp);
  fclose(fp);
  printer();
}
