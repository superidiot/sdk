#include <stdio.h>
#include "sudoku.h"

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

static rem_n_at(int i, int j, struct s *sp){
  int k;
  int n = sp->a[i][j].n;
  for (k = 0; k < 9; k++){
    if (sp->a[i][k].ns[n] != 0 && sp->a[i][k].ns[0] > 0){
      sp->a[i][k].ns[n] = 0;
      sp->a[i][k].ns[0]--;
    }
    if (sp->a[k][i].ns[n] != 0 && sp->a[i][k].ns[0] > 0){
      sp->a[k][i].ns[n] = 0;
      sp->a[k][i].ns[0]--;
    }
  }
}

static rem_n_from_frees(struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (sp->a[i][j].n != 0)
	rem_n_at(i,j,sp);
    }
  }
}

void solver(struct s *sp){
  init_support(sp);
  rem_n_from_frees(sp);
}
