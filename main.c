#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

/* TODO:
 * 1. free the mallocs of reader.c
 * 2. Add option to interactive mode, so that ps can be printed at the given state.
 * 3. implement golden chains
 */

int main (int argc, char *argv[])
{
  int i, c, interactive, solved;
  solved = -1;
  c = 0;
  interactive = 0;
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
        interactive = TRUE;
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
    }
  return solved;
}

