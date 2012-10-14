#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
enum bool {false, true};
enum bool changed;
static struct f *tmp[9];

static struct desk {
  // fields contains points to row, column or subsquare
  struct f *fields[9];
  // index contains index of row, column or subsquare
  int index;
  // type contains 'r' for row, 'c' for column, 's' for subsquare
  char type;
} desktop;

// determines the hamming-weight of ns
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

/* returns true if ns contains n, else false */
static enum bool contains(int ns, int n){
  return ( (ns & (1 << (n - 1)) > 0) ? true : false);
}

// get pointer to field a[i][j]
static struct f *get_fp(struct s *sp, int i, int j){
  return &(sp->a[i][j]);
}

// k specifies the row
static void load_row(struct s *sp, int k){
  int i;
  desktop.type = 'r';
  desktop.index = k;
  for (i = 0; i < 9; i++){
    desktop.fields[i] = &(sp->a[k][i]);
  }
}

// k specifies the column
static load_col(struct s *sp, int k){
  int i;
  desktop.type = 'c';
  desktop.index = k;
  for (i = 0; i < 9; i++){
    desktop.fields[i] = &(sp->a[i][k]);
  }
}

// k specifies subsquare k when traversing the sudoku in z-shape
static load_squ(struct s *sp, int k){
  int i,j,u,v,c;
  c = 0;
  i = 3 * (k / 3);
  j = 3 * (k % 3);
  desktop.type = 's';
  desktop.index = k;
  for (u = 0; u < 3; u++){
    for (v = 0; v < 3; v++){
      desktop.fields[c++] = &(sp->a[i + u][j + v]);
    }
  }
}

// removes number n from current tmp-array
static void rem_n_tmp(int n){
  int i;
  for (i = 0; i < 9; i++){
    desktop.fields[i]->ns &= ~(1 << (n-1));
  }
}

// determines subsquare-number k that contains index (i,j)
static int get_squ_number(int i, int j){
  int ret;
  if (i < 3 && j < 3) ret = 0;
  else if (i < 3 && j < 6) ret = 1;
  else if (i < 3) ret = 2;
  else if (i < 6 && j < 3) ret = 3;
  else if (i < 6 && j < 6) ret = 4;
  else if (i < 6) ret = 5;
  else if (j < 3) ret = 6;
  else if (j < 6) ret = 7;
  else ret = 8;
  return ret;
}

// removes n from ns at (i,j) in row, column, and subsquare
static void rem_n_at(struct s *sp, int i, int j){
  if (sp->a[i][j].n != 0){
    load_row(sp,i);
    rem_n_tmp(sp->a[i][j].n);
    load_col(sp,j);
    rem_n_tmp(sp->a[i][j].n);
    load_squ(sp, get_squ_number(i, j) );
    rem_n_tmp(sp->a[i][j].n);
  }
}

// removes n from ns in row, column and subsquare for each entry
static void init_ns(struct s *sp){
  int i, j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      rem_n_at(sp, i, j);
    }
  }
}

// sets n at (i,j). Should only be called when popcount(ns) == 1
static void set_single(struct s *sp, int i, int j){
  int k,tmp;
  for (k = 0; k < 9; k++){
    if ((1 << k) == sp->a[i][j].ns){
      sp->a[i][j].ns = 0;
      sp->a[i][j].n = k + 1;
      rem_n_at(sp, i, j);
      changed = true;
    }
  }
}

// sets all fields that have popcount(ns) == 1
static void set_singles(struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (popcount(sp->a[i][j].ns) == 1){
	set_single(sp,i,j);
      }
    }
  }
}

// if uniq n was found in tmp, set it
static void set_uniq_tmp(int n){
  int i;
  for (i = 0; i < 9; i++){
    if ( (1 << (n - 1)) == (desktop.fields[i]->ns & (1 << (n - 1))) ){
      desktop.fields[i]->n = n;
      desktop.fields[i]->ns = 0;
      changed = true;
    }
  }
}

// find a uniq in tmp, call the method to set it
static void find_uniq_tmp(){
  int i,n,c;
  for (n = 0; n < 9; n++){
    c = 0;
    for (i = 0; i < 9; i++){
      //printf("checking %d\n", n);
      if ((desktop.fields[i]->ns & (1 << n)) == (1 << n)){
	c++;
      }
    }
    if (c == 1){
      printf("found uniq %d!\n", (n + 1)); 
      set_uniq_tmp(n + 1);
    }
  }
}

// find uniqs in rows, columns and subsquares.  Also set them.
static void set_uniqs(struct s *sp){
  int k;
  for (k = 0; k < 9; k++){
    load_row(sp, k);
    printf("looking for uniq in row %d\n", k);
    find_uniq_tmp( );
    load_col(sp, k);
    printf("looking for uniq in column %d\n", k);
    find_uniq_tmp( );
    load_squ(sp, k);
    printf("looking for uniq in square %d\n", k);
    find_uniq_tmp( );
  }
}

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
  changed = true;
  int i,j;
  init_ns(sp);
  do {
    changed = false;
    set_singles(sp);
    set_uniqs(sp);
  } while (changed == true);
}
