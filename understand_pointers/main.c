#include <stdio.h>
#include <stdlib.h>


int ** testfunction()
{
  int **ar = malloc(9*sizeof(int *));
  int i;
  for (i = 0; i < 9; i++)
    {
      *(ar+i) = malloc(sizeof(int));
      **(ar+ i) = i;
    }
  return ar;
}

main (int argc, char *argv[])
{
  int j;
  int **ar = testfunction();

  for (j=0;j<9;j++)
    {
      printf("lolwut: %d\n", **(ar+j));
    }
}
