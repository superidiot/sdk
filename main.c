#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int main (int argc, char *argv[])
{
  int i, c, interactive, solved;
  solved = -1;
  c = 0;
  char file[90];
  file[0] = 0;
  while ( --argc > 0  && (*++argv)[0] == '-' )
    {
      c = *++argv[0];
      switch (c){
      case 'f':
        i = 0;
        while ( (file[i++] = *argv[1]++) != '\0') ;
        argc--;
        argv++;
        break;
      case 'i':
        interactive = 1;
        break;
      case 'h':
        printf("%s", "Options for sudoku:\n");
        printf("%s", "-f file: specify sudoku-file. This is not optional\n");
        printf("%s", "-i run interactive\n");
        printf("%s", "-h: this message\n");
        break;
      default:
        argc = 0;
        break;
      }
    }

  if (file[0] == 0 || argc != 0)
    {
      printf("%s", "You should give me a sudoku file.\n");
      printf("%s", "Run this command with -h for more information.\n");
    }
  else
    {
      /* int i,j; */
      FILE *fp = fopen(file,"r");
      struct s *sp;
      sp = reader(fp);
      fclose(fp);
      printer_cli(sp);
      /* printer_ps(sp, "sudoku.ps", 1, 11, "w"); */
      solved = solver(sp, interactive);
      printer_cli(sp);
    /*   for (i = 0; i < 9; i++) */
    /*     { */
    /*       for (j = 0; j < 9; j++) */
    /*         { */
    /*           printf("%d",MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->n); */
    /*         } */
    /*       printf("%s", "\n"); */
    /*     } */
    /*   printf("%s","=========\n"); */
    /*   for (i = 0; i < 9; i++) */
    /*     { */
    /*       for (j = 0; j < 9; j++) */
    /*         { */
    /*           printf("%d",MATRIX_ROW_MAJOR_IDX(sp->transposed, 9, i, j)->n); */
    /*         } */
    /*       printf("%s", "\n"); */
    /*     } */
    /*   printf("%s","=========\n"); */
    /*   for (i = 0; i < 9; i++) */
    /*     { */
    /*       for (j = 0; j < 9; j++) */
    /*         { */
    /*           printf("%d",MATRIX_ROW_MAJOR_IDX(sp->transformed, 9, i, j)->n); */
    /*         } */
    /*       printf("%s", "\n"); */
    /*     } */
    /*   printf("%s","=========\n"); */
    /*   for (i = 0; i < 9; i++) */
    /*          { */
    /*            for (j = 0; j < 9; j++) */
    /*              { */
    /*                printf("%d",MATRIX_ROW_MAJOR_IDX(sp->transforposed, 9, i, j)->n); */
    /*              } */
    /*            printf("%s", "\n"); */
    /*          } */
    }
  return solved;
}

