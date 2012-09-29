#include <stdio.h>

#include "sudoku.h"

void printer (struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    if (i % 3 == 0){
      printf("%s","+-----------+\n");
    }
    for (j = 0; j < 9; j++){
      if (j % 3 == 0){
	printf("%s","|");
      }
      printf("%1d", sp->a[i][j].n);
    }
    printf("%1s","|\n");
  }
  printf("%s","+-----------+\n");
  
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      int k;
      int tmp;
      for (k = 0; k < 10; k++){
	tmp = sp->a[i][j].ns[k];
	if (tmp == 0)
	  printf("%s", "_");
	else
	  printf("%1d",tmp);
      }
      printf("%s"," ");
    }
    printf("%s","\n");
  }
}
