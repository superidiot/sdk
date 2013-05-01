#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

struct s* reader(FILE *fp)
{
  int i,j,tmpi,tmpj;
  extern struct s sudoku;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
        {
          MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j) =
            (struct f *) malloc(sizeof(struct f));

	  MATRIX_COL_MAJOR_IDX(sudoku.transposed, 9, i, j) =
	    MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j);

          tmpi =  (j / 3);
          tmpj = (j % 3) + ((i%3) * 3);
          if (i % 3 == j/3){
            /* printf("Transform1 (%d,%d)->(%d,%d)\n", i,j,i,j); */
            MATRIX_ROW_MAJOR_IDX(sudoku.transformed, 9, i, j) =
              MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j);
          } else if (i % 3 < j / 3)
            {
              {
                tmpi = (j / 3) + (i / 3)*3;
                tmpj = j % 3 + (i % 3) * 3;
                /* printf("Transform2 (%d,%d)->(%d,%d)\n", i,j,tmpi,tmpj); */
                MATRIX_ROW_MAJOR_IDX(sudoku.transformed, 9, tmpi, tmpj) =
                  MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j);
              }
            } else
            {
              tmpi = (j / 3) + (i / 3)*3;
              tmpj = (i % 3) * 3 + j % 3;
              /* printf("Transform3 (%d,%d)->(%d,%d)\n", i,j,tmpi,tmpj); */
              MATRIX_ROW_MAJOR_IDX(sudoku.transformed, 9, tmpi, tmpj) =
                MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j);
            }

          /* MATRIX_COL_MAJOR_IDX(sudoku.transforposed, 9, i, j) =  */
          /*   MATRIX_ROW_MAJOR_IDX(sudoku.transformed, 9, i, j); */

          fscanf(fp, "%1d", &(MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j)->n));

          if (MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j)->n == 0)
            {
              MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j)->ns =
                ONE   |
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
              MATRIX_ROW_MAJOR_IDX(sudoku.normal, 9, i, j)->ns = 0;
            }
        }
    }
  return &sudoku;
}
