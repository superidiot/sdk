#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

struct s* reader(FILE *fp){
  int i,j;
  extern struct s sudoku;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      struct f *field = (struct f *) malloc (sizeof(struct f));
      fscanf(fp, "%1d", &(field->n));
      if (field->n == 0){
	field->ns = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE;
      }
      else {
	field->ns = 0;
      }
	
      sudoku.a[i][j] = field;
    }
  }
  return &sudoku;
}
