#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
enum bool {false = 0, true = 1};
enum bool changed;
static struct f *tmp[9];
enum bool interactive = false;

static struct desk {
  // fields contains points to row, column or subsquare
  struct f *fields[9];
  // index contains index of row, column or subsquare
  int index;
  // type contains 'r' for row, 'c' for column, 's' for subsquare
  char type;
} desktop;

// determines the hamming-weight of ns
static int popcount(int ns){
  int c,i;
  c = 0;
  for (i = 0; i < 9; i++){
    if ( (ns & (1 << i)) == (1 << i) ){
      c++;
    }
  }
  return c;
}

/* returns true if ns contains n, else false */
static enum bool contains(int ns, int n){
  return ( (ns & (1 << (n - 1))) > 0 ? true : false);
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
    if ( contains(desktop.fields[i]->ns, n) ){
      desktop.fields[i]->ns &= ~(1 << (n - 1));
      changed = true;
    }
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
  load_row(sp,i);
  rem_n_tmp(sp->a[i][j].n);
  load_col(sp,j);
  rem_n_tmp(sp->a[i][j].n);
  load_squ(sp, get_squ_number(i, j) );
  rem_n_tmp(sp->a[i][j].n);
}

static void set_n_at(struct s *sp, int i, int j, int n){
  if (interactive){
    printf("setting %d at (%d,%d)\n", n, i, j);
    getchar();
  }
  sp->a[i][j].n = n;
  sp->a[i][j].ns = 0;
  rem_n_at(sp, i, j);
  changed = true;
  if (interactive){
    printer_cli(sp);
  }	
}

// removes n from ns in row, column and subsquare for each entry
static void init_ns(struct s *sp){
  int i, j;
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (sp->a[i][j].n != 0){
	rem_n_at(sp, i, j);
      }
    }
  }
}

// sets n at (i,j). Should only be called when popcount(ns) == 1
static void set_single(struct s *sp, int i, int j){
  int k,tmp;
  for (k = 0; k < 9; k++){
    if ((1 << k) == sp->a[i][j].ns){
      set_n_at(sp, i, j, (k + 1));
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
static void set_uniq_tmp(struct s *sp, int n){
  int i;
  int x,y,u,v;
  for (i = 0; i < 9; i++){
    if ( contains(desktop.fields[i]->ns, n) ){
      switch (desktop.type){
      case 'r':
	//printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, desktop.index, i, desktop.type, desktop.index);
	set_n_at(sp, desktop.index, i, n);
	break;
      case 'c':
	//printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, i, desktop.index, desktop.type, desktop.index);
	set_n_at(sp, i, desktop.index, n);
	break;
      case 's':
	  x = 3 * (desktop.index / 3);
	  y = 3 * (desktop.index % 3);
	  u = i / 3;
	  v = i % 3;
	  //printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, x + u, y + v,desktop.type, desktop.index);
	  set_n_at(sp, x + u, y + v, n);
	break;
      }
    }
  }
}

// find a uniq in tmp, call the method to set it
static void find_uniq_tmp(struct s *sp){
  int i,n,c;
  for (n = 0; n < 9; n++){
    c = 0;
    for (i = 0; i < 9; i++){
      if ( contains(desktop.fields[i]->ns, (n + 1)) ){
	c++;
      }
    }
    if (c == 1){
      //printf("found uniq %d\n", n + 1);
      set_uniq_tmp(sp, n + 1);
    }
  }
}

// find uniqs in rows, columns and subsquares.  Also set them.
static void set_uniqs(struct s *sp){
  int k;
  for (k = 0; k < 9; k++){
    load_row(sp, k);
    find_uniq_tmp(sp);
    load_col(sp, k);
    find_uniq_tmp(sp);
    load_squ(sp, k);
    find_uniq_tmp(sp);
  }
}

static int test(struct s *sp){
  int i,j,p,s,ret,c;
  for (i = 0; i < 9; i++){
    s = 0;
    p = 1;
    for (j = 0; j < 9; j++){
      s += sp->a[i][j].n;
      p *= sp->a[i][j].n;
    }
    if (p == 362880 && s == 45){
      c++;
    }
  }
  return (c == 18 ? 0 : -1);
}

int solver(struct s *sp, int inter){
  changed = true;
  interactive = inter;
  int i,j;
  init_ns(sp);
  do {
    changed = false;
    set_singles(sp);
    set_uniqs(sp);
  } while (changed == true);
  return test(sp);
}
