#include <stdio.h>
#include "sudoku.h"

void reader(FILE *fp, struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      fscanf(fp, "%1d", &(sp->a[i][j].n));
      int k;
      int tmps[10];
      for (k = 0; k < 10; k++){
	sp->a[i][j].ns[k] = k;
      }
    }
  }
}
