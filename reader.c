#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
/* http://en.wikipedia.org/wiki/Row-major_order */

#define MATRIX_ROW_MAJOR_IDX(A, ncol, row, col) A[row * ncol + col] /* offset = row*NUMCOLS + column. */
#define MATRIX_COL_MAJOR_IDX(A, nrow, row, col) A[row + col * nrow] /* offset = row + column*NUMROWS. */

struct s* reader(FILE *fp)
{
  int i,j;
  extern struct s sudoku;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  fscanf(fp, "%1d", &(sudoku.normal[i][j].n));
	  if (sudoku.normal[i][j].n == 0)
	    {
	      sudoku.normal[i][j].ns = ONE   |
		                  TWO   |
		                  THREE |
		                  FOUR  |
		                  FIVE  |
		                  SIX   |
		                  SEVEN |
		                  EIGHT |
		                  NINE;
	    }
	  else
	    {
	      sudoku.normal[i][j].ns = 0;
	    }
	}
    }
  return &sudoku;
}
