#include <stdio.h>

#include "sudoku.h"

void printer (){
  int i,j;
  for (i = 0; i < 9; i++){
    if (i % 3 == 0){
      printf("%s","+-----------+\n");
    }
    for (j = 0; j < 9; j++){
      if (j % 3 == 0){
	printf("%s","|");
      }
      printf("%1d", sudoku.a[i][j].n);
    }
    printf("%1s","|\n");
  }
  printf("%s","+-----------+\n");
}
