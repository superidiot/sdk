#include <stdio.h>

struct f {
  int n;
  int ns[10];
  };

struct s {
  struct f a[9][9];
} sudoku;

void reader(FILE *fp){
  int i,j,k;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      fscanf(fp, "%1d", &(sudoku.a[i][j].n));
      for (k = 0; k < 10; k++){
	sudoku.a[i][j].ns[k] = k;
      }
    }
  }
}

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

main (){
  FILE *fp = fopen("sudoku.txt","r");
  reader(fp);
  fclose(fp);
  printer();
}
