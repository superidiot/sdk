#include <stdio.h>

#include "sudoku.h"

void reader(FILE *fp){
  int i,j,k;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      fscanf(fp, "%1d", &(sudoku.a[i][j].n));
      for (k = 0; k < 10; k++){
	sudoku.a[i][j].ns[k] = k;
      }
    }
  }
}
