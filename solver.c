#include <stdio.h>
#include "sudoku.h"
static short changes = 0;

/* set first index of ns to number of non-zero entries*/
static void support(int *ns){
  int i;
  for (i = 0; i < 10; i++){
    if (ns[i] != 0){
      ++ns[0];
    }
  }
}

static init_support(struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (sp->a[i][j].n == 0)
	support(sp->a[i][j].ns);
      else {
	int k;
	for (k = 0; k < 10; k++){
	  sp->a[i][j].ns[k] = 0;
	}
      }
    }
  }
}

/* returns 0 if ns contains n, else 1 */
static int contains(int *ns, int n){
  int k;
  for (k = 0; k < 10; k++){
    if (ns[k] == n)
      return 0;
  }
  return 1;
}

static void rem_n_square(int i, int j, struct s *sp){
  int k,l,u,v,tmp;
  tmp = sp->a[i][j].n;
  /*  u = 3 * (i % 3);
      v = 3 * (j / 3);*/
  if (i < 3) u = 0;
  else if ((i >= 3) && (i < 6)) u = 3;
  else if (i >= 6) u = 6;
  if (j < 3) v = 0;
  else if ((j >= 3) && (j < 6)) v = 3;
  else if (j >= 6) v = 6;
  /*printf("removing %d at square (%d,%d)\n",tmp,u,v);*/
  for (k = 0; k < 3; k++){
    for (l = 0; l < 3; l++){
      if ((sp->a[k+u][l+v].ns[tmp] > 0) && (sp->a[k+u][l+v].ns[0] > 0)){
	sp->a[k+u][l+v].ns[tmp] = 0;
	sp->a[k+u][l+v].ns[0]--;
	changes = 1;
      }
    }
  }
}

static rem_n_at(int i, int j, struct s *sp){
  int k;
  int tmp = sp->a[i][j].n;
  /*  printf("removing %d at (%d,%d)\n",tmp,i,j);*/
  for (k = 0; k < 9; k++){
    if ((sp->a[i][k].ns[tmp] > 0) && (sp->a[i][k].ns[0] > 0)){
      sp->a[i][k].ns[tmp] = 0;
      sp->a[i][k].ns[0]--;
      changes = 1;
    }
    if ((sp->a[k][j].ns[tmp] > 0) && (sp->a[k][j].ns[0] > 0)){
      sp->a[k][j].ns[tmp] = 0;
      sp->a[k][j].ns[0]--;
      changes = 1;
    }
  }
  rem_n_square(i,j,sp);
}

static rem_n_from_frees(struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (sp->a[i][j].n > 0)
	rem_n_at(i,j,sp);
    }
  }
}

static set_single(int i, int j, struct s *sp){
  int k,tmp;
  for (k = 1; k < 10; k++){
    tmp = sp->a[i][j].ns[k];
    if (tmp != 0){
      sp->a[i][j].n = tmp;
      sp->a[i][j].ns[0] = 0;
      sp->a[i][j].ns[tmp] = 0;
      rem_n_at(i,j,sp);
      changes = 1;
    }
  }
}

static set_singles(struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (sp->a[i][j].ns[0] == 1)
	set_single(i,j,sp);
    }
  }
}

void solver(struct s *sp){
  init_support(sp);
  do {
    changes = 0;
    rem_n_from_frees(sp);
    set_singles(sp);
  }
  while (changes);
}
