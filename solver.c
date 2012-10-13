#include <stdio.h>
#include "sudoku.h"
enum bool {false, true};

static int popcount (int ns){
  int c = 0;
  if ((ns | ONE) == ns )  { ++c; }
  if ((ns | TWO) == ns)   { ++c; }
  if ((ns | THREE) == ns) { ++c; }
  if ((ns | FOUR) == ns)  { ++c; }
  if ((ns | FIVE) == ns)  { ++c; }
  if ((ns | SIX) == ns)   { ++c; }
  if ((ns | SEVEN) == ns) { ++c; }
  if ((ns | EIGHT) == ns) { ++c; }
  if ((ns | NINE) == ns)  { ++c; }
  return c;
}

/* static init_support(struct s *sp){ */
/*   int i,j; */
/*   for (i = 0; i < 9; i++) { */
/*     for (j = 0; j < 9; j++) { */
/*       if (sp->sudoku_array[i][j].n == 0) */
/* 	support(sp->sudoku_array[i][j].ns); */
/*       else { */
/* 	int k; */
/* 	for (k = 0; k < 10; k++){ */
/* 	  sp->sudoku_array[i][j].ns[k] = 0; */
/* 	} */
/*       } */
/*     } */
/*   } */
/* } */

/* returns true if ns contains n, else false */
static enum bool contains(int ns, int n){
  return ( (ns | (1 << n) == ns) ? true : false);
}

// k specifies the row
static struct f *get_row(struct s *sp, int k){
  return sp->a[k][0];
}

// k specifies the column
static struct f *get_col(int k){
}

// k specifies subsquare k
static struct f *get_squ(int k){
}

/* static void rem_n_square(int i, int j, struct s *sp){ */
/*   int k,l,u,v,tmp; */
/*   tmp = sp->sudoku_array[i][j].n; */
/*   /\*  u = 3 * (i % 3); */
/*       v = 3 * (j / 3);*\/ */
/*   if (i < 3) u = 0; */
/*   else if ((i >= 3) && (i < 6)) u = 3; */
/*   else if (i >= 6) u = 6; */
/*   if (j < 3) v = 0; */
/*   else if ((j >= 3) && (j < 6)) v = 3; */
/*   else if (j >= 6) v = 6; */
/*   /\*printf("removing %d at square (%d,%d)\n",tmp,u,v);*\/ */
/*   for (k = 0; k < 3; k++){ */
/*     for (l = 0; l < 3; l++){ */
/*       if ((sp->sudoku_array[k+u][l+v].ns[tmp] > 0) && (sp->sudoku_array[k+u][l+v].ns[0] > 0)){ */
/* 	sp->sudoku_array[k+u][l+v].ns[tmp] = 0; */
/* 	sp->sudoku_array[k+u][l+v].ns[0]--; */
/* 	changes = 1; */
/*       } */
/*     } */
/*   } */
/* } */

/* /\*removes the number at (i,j) from row, column and square*\/ */
/* static rem_n_at(int i, int j, struct s *sp){ */
/*   int k; */
/*   int tmp = sp->sudoku_array[i][j].n; */
/*   /\*printf("removing %d at (%d,%d)\n",tmp,i,j);*\/ */
/*   for (k = 0; k < 9; k++){ */
/*     if ((sp->sudoku_array[i][k].ns[tmp] > 0) && (sp->sudoku_array[i][k].ns[0] > 0)){ */
/*       sp->sudoku_array[i][k].ns[tmp] = 0; */
/*       sp->sudoku_array[i][k].ns[0]--; */
/*       changes = 1; */
/*     } */
/*     if ((sp->sudoku_array[k][j].ns[tmp] > 0) && (sp->sudoku_array[k][j].ns[0] > 0)){ */
/*       sp->sudoku_array[k][j].ns[tmp] = 0; */
/*       sp->sudoku_array[k][j].ns[0]--; */
/*       changes = 1; */
/*     } */
/*   } */
/*   rem_n_square(i,j,sp); */
/* } */

/* static rem_n_from_frees(struct s *sp){ */
/*   int i,j; */
/*   for (i = 0; i < 9; i++){ */
/*     for (j = 0; j < 9; j++){ */
/*       if (sp->sudoku_array[i][j].n > 0) */
/* 	rem_n_at(i,j,sp); */
/*     } */
/*   } */
/* } */

/* static set_single(int i, int j, struct s *sp){ */
/*   int k,tmp; */
/*   for (k = 1; k < 10; k++){ */
/*     tmp = sp->sudoku_array[i][j].ns[k]; */
/*     if (tmp != 0){ */
/*       sp->sudoku_array[i][j].n = tmp; */
/*       sp->sudoku_array[i][j].ns[0] = 0; */
/*       sp->sudoku_array[i][j].ns[tmp] = 0; */
/*       rem_n_at(i,j,sp); */
/*       changes = 1; */
/*     } */
/*   } */
/* } */

/* static set_singles(struct s *sp){ */
/*   int i,j; */
/*   for (i = 0; i < 9; i++){ */
/*     for (j = 0; j < 9; j++){ */
/*       if (sp->sudoku_array[i][j].ns[0] == 1) */
/* 	set_single(i,j,sp); */
/*     } */
/*   } */
/* } */

/* void uniqs(struct s *sp){ */
/*   int i,j,k,l,c; */
/*   /\* first only look at columns *\/ */
/*   for (j = 0; j < 9; j++){ */
/*     for (k = 1; k < 10; k++){ */
/*       c = 0; */
/*       for (i = 0; i < 9; i++){ */
/* 	if ((sp->sudoku_array[i][j].n == 0) && (sp->sudoku_array[i][j].ns[k] == k)){ */
/* 	  c++; */
/* 	} */
/*       } */
/*       if (c == 1){ */
/* 	for (i = 0; i < 9; i++){ */
/* 	  if (sp->sudoku_array[i][j].ns[k] == k){ */
/* 	    sp->sudoku_array[i][j].n = k; */
/* 	    for (l = 0; l < 10; l++){ */
/* 	      sp->sudoku_array[i][j].ns[l] = 0; */
/* 	    } */
/* 	    rem_n_at(i,j,sp); */
/* 	  } */
/* 	} */
/*       } */
/*     } */
/*   } */
/*   /\* then look at rows*\/ */
/*   for (i = 0; i < 9; i++){ */
/*     for (k = 1; k < 10; k++){ */
/*       c = 0; */
/*       for (j = 0; j < 9; j++){ */
/* 	if ((sp->sudoku_array[i][j].n == 0) && (sp->sudoku_array[i][j].ns[k] == k)){ */
/* 	  c++; */
/* 	} */
/*       } */
/*       if (c == 1){ */
/* 	for (j = 0; j < 9; j++){ */
/* 	  if (sp->sudoku_array[i][j].ns[k] == k){ */
/* 	    sp->sudoku_array[i][j].n = k; */
/* 	    for (l = 0; l < 10; l++){ */
/* 	      sp->sudoku_array[i][j].ns[l] = 0; */
/* 	    } */
/* 	    rem_n_at(i,j,sp); */
/* 	  } */
/* 	} */
/*       } */
/*     } */
/*   } */
/* } */

void solver(struct s *sp){
  int changed = true;
  int i;
  struct f *tmp;
  //init_support(sp);
  do {
    changed = false;
    printf("%d", popcount(sp->a[0][1]->ns));
    sp->a[0][1]->ns = sp->a[0][1]->ns ^ ONE;
    printf("%d", popcount(sp->a[0][1]->ns));
    printf("%d", contains(sp->a[0][0]->ns, 1));
    printf("%d", contains(sp->a[0][0]->ns, 5));
    printf("%d", contains(sp->a[0][1]->ns, 5));
    printf("%d\n", contains(sp->a[0][1]->ns, 1));
    tmp = get_row(sp, 0);
    for (i = 0; i < 9; i++){
      printf("%d", tmp[i].n);
    }
    // rem_n_from_frees(sp);
    // set_singles(sp);
    // uniqs(sp);
  }
  while (changed);
}
