#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
enum bool {false = 0, true = 1};
enum bool changed;
enum bool interactive = false;

static struct desk
{
  /* fields contains points to row, column or subsquare */
  struct f *fields[9];
  /* index contains index of row, column or subsquare */
  int index;
  /* type contains 'r' for row, 'c' for column, 's' for subsquare */
  char type;
} desktop;

/* determines the hamming-weight of ns */
static int popcount(int ns)
{
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
static enum bool contains(int ns, int n)
{
  return ( (ns & (1 << (n - 1))) > 0 ? true : false);
}

/* get pointer to field a[i][j] */
static struct f *get_fp(struct s *sp, int i, int j)
{
  return &(sp->a[i][j]);
}

/* k specifies the row */
static void load_row(struct s *sp, int k)
{
  int i;
  desktop.type = 'r';
  desktop.index = k;
  for (i = 0; i < 9; i++)
    {
      desktop.fields[i] = &(sp->a[k][i]);
    }
}

/* k specifies the column */
static load_col(struct s *sp, int k)
{
  int i;
  desktop.type = 'c';
  desktop.index = k;
  for (i = 0; i < 9; i++)
    {
      desktop.fields[i] = &(sp->a[i][k]);
    }
}

/* k specifies subsquare k when traversing the sudoku in z-shape */
static load_squ(struct s *sp, int k)
{
  int i,j,u,v,c;
  c = 0;
  i = 3 * (k / 3);
  j = 3 * (k % 3);
  desktop.type = 's';
  desktop.index = k;
  for (u = 0; u < 3; u++)
    {
      for (v = 0; v < 3; v++)
	{
	  desktop.fields[c++] = &(sp->a[i + u][j + v]);
	}
    }
}

/* removes number n from current tmp-array */
static void rem_n_tmp(int n)
{
  int i;
  for (i = 0; i < 9; i++)
    {
      if ( contains(desktop.fields[i]->ns, n) )
	{
	  desktop.fields[i]->ns &= ~(1 << (n - 1));
	  changed = true;
	}
    }
}

/* determines subsquare-number k that contains index (i,j) */
static int get_squ_number(int i, int j)
{
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

/* removes n from ns at (i,j) in row, column, and subsquare */
static void rem_n_at(struct s *sp, int i, int j)
{
  load_row(sp,i);
  rem_n_tmp(sp->a[i][j].n);
  load_col(sp,j);
  rem_n_tmp(sp->a[i][j].n);
  load_squ(sp, get_squ_number(i, j) );
  rem_n_tmp(sp->a[i][j].n);
}

static void set_n_at(struct s *sp, int i, int j, int n)
{
  if (interactive)
    {
      printf("setting %d at (%d,%d)\n", n, i, j);
      getchar();
    }
  sp->a[i][j].n = n;
  sp->a[i][j].ns = 0;
  rem_n_at(sp, i, j);
  changed = true;
  if (interactive)
    {
      printer_cli(sp);
    }
}

/* removes n from ns in row, column and subsquare for each entry */
static void init_ns(struct s *sp)
{
  int i, j;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  if (sp->a[i][j].n != 0)
	    {
	      rem_n_at(sp, i, j);
	    }
	}
    }
}

/* sets n at (i,j). Should only be called when popcount(ns) == 1 */
static void set_single(struct s *sp, int i, int j)
{
  int k,tmp;
  for (k = 0; k < 9; k++)
    {
      if ((1 << k) == sp->a[i][j].ns)
	{
	  set_n_at(sp, i, j, (k + 1));
	}
    }
}

/* sets all fields that have popcount(ns) == 1 */
static void set_singles(struct s *sp)
{
  int i,j;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  if (popcount(sp->a[i][j].ns) == 1)
	    {
	      set_single(sp,i,j);
	    }
	}
    }
}

/* if uniq n was found in tmp, set it */
static void set_uniq_tmp(struct s *sp, int n)
{
  int i;
  int x,y,u,v;
  for (i = 0; i < 9; i++)
    {
      if ( contains(desktop.fields[i]->ns, n) )
	{
	  switch (desktop.type)
	    {
	    case 'r':
	      /* printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, desktop.index, i, desktop.type, desktop.index); */
	      set_n_at(sp, desktop.index, i, n);
	      break;
	    case 'c':
	      /* printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, i, desktop.index, desktop.type, desktop.index); */
	      set_n_at(sp, i, desktop.index, n);
	      break;
	    case 's':
	      x = 3 * (desktop.index / 3);
	      y = 3 * (desktop.index % 3);
	      u = i / 3;
	      v = i % 3;
	      /* printf("set_uniq_tmp: found %d at (%d,%d) processing [%c %d]\n", n, x + u, y + v,desktop.type, desktop.index); */
	      set_n_at(sp, x + u, y + v, n);
	      break;
	    }
	}
    }
}

/* find a uniq in tmp, call the method to set it */
static void find_uniq_tmp(struct s *sp)
{
  int i,n,c;
  for (n = 0; n < 9; n++)
    {
      c = 0;
      for (i = 0; i < 9; i++)
	{
	  if ( contains(desktop.fields[i]->ns, (n + 1)) )
	    {
	      c++;
	    }
	}
      if (c == 1)
	{
	  /* printf("found uniq %d\n", n + 1); */
	  set_uniq_tmp(sp, n + 1);
	}
    }
}

/* find uniqs in rows, columns and subsquares.  Also set them. */
static void set_uniqs(struct s *sp)
{
  int k;
  for (k = 0; k < 9; k++)
    {
      load_row(sp, k);
      find_uniq_tmp(sp);
      load_col(sp, k);
      find_uniq_tmp(sp);
      load_squ(sp, k);
      find_uniq_tmp(sp);
    }
}

/* returns number of occurences of n in ns for desktop.fields */
static int fields_ns_contain(int n)
{
  int i,c;
  c = 0;
  for (i = 0; i < 9; i++)
    {
      if (contains(desktop.fields[i]->ns, n))
	{
	  c++;
	}
    }
}

/* should find 3 in square 1, row 2 and delete it from square 3 in sudoku2.txt */
static void find_tupel(struct s *sp)
{
  int i,j,c,d,u,v,n,tupel,tupel_n;
  load_row(sp, 2);
  /* tupel determines the startpoints of the three tripels in desktop.fields */
  for (tupel = 0; tupel < 9; tupel += 3)
    {
      /* tupel_n are the numbers to look for in each tripel */
      for (tupel_n = 0; tupel_n < 9; tupel_n++)
	{
	  c = 0;
	  /* i iterates over the tripel */
	  for (i = tupel; i < tupel + 3; i++)
	    {
	      if ( contains(desktop.fields[i]->ns, tupel_n) )
		{
		  c++;
		}
	    }
	  if (c > 0)
	    {
	      printf("found candidate %d at tupel %d in row 2\n", tupel_n, tupel);
	    }
	}
    }
}
/* for (j = 0; j < 9; j++)
   { */
/*   c = 0; */
/* for (i = 3; i < 6; i++)
   { */
/*   if (contains(sp->a[2][i].ns, j + 1))
     { */
/* 	c++; */
/*   } */
/* } */
/* if (c > 1)
   { */
/*   printf("possibly found tupel of %d\n", j + 1); */
/*   load_squ(sp, 1); */
/*   d = 0; */
/*   for (u = 0; u < 6; u++)
     { */
/* 	if (contains(desktop.fields[u]->ns, j + 1))
	{ */
/* 	  d++; */
/* 	} */
/*   } */
/*   if (d > 0)
     { */
/* 	printf("%d is not what we want\n", j + 1); */
/*   } else if (d == 0)
     { */
/* 	printf("%d is very nice\n", j + 1); */
/* 	load_row(sp, 2); */
/* 	for (v = 0; v < 3; v++)
	{ */
/* 	  if (contains(desktop.fields[v]->ns, j + 1))
	  { */
/* 	    desktop.fields[v]->ns &= ~(1 << j); */
/* 	    changed = true; */
/* 	  } */
/* 	} */
/* 	for (v = 6; v < 9; v++)
	{ */
/* 	  if (contains(desktop.fields[v]->ns, j + 1))
	  { */
/* 	    desktop.fields[v]->ns &= ~(1 << j); */
/* 	    changed = true; */
/* 	  } */
/* 	} */
/*   } */
/* } */
/* } */
/* } */

/* test sum and product of rows */
static int test(struct s *sp)
{
  int i,j,p,s,ret,c;
  c = 0;
  for (i = 0; i < 9; i++)
    {
      s = 0;
      p = 1;
      for (j = 0; j < 9; j++)
	{
	  s += sp->a[i][j].n;
	  p *= sp->a[i][j].n;
	}
      if (p == 362880 && s == 45)
	{
	  c++;
	}
    }
  return (c == 9 ? 0 : -1);
}

int solver(struct s *sp, int inter)
{
  changed = true;
  interactive = inter;
  int i,j;
  init_ns(sp);
  do
    {
      changed = false;
      set_singles(sp);
      set_uniqs(sp);
      /* find_tupel(sp); */
    }
  while (changed == true);
  find_tupel(sp);
  return test(sp);
}
