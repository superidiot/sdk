#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

struct s* reader(FILE *fp)
{
  int i,j;
  extern struct s sudoku;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  /* struct f *field = (struct f *) malloc (sizeof(struct f)); */
	  fscanf(fp, "%1d", &(sudoku.a[i][j].n));
	  if (sudoku.a[i][j].n == 0)
	    {
	      sudoku.a[i][j].ns = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE;
	    }
	  else
	    {
	      sudoku.a[i][j].ns = 0;
	    }
	}
    }
  return &sudoku;
}
