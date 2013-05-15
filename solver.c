#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "dbg.h"

int changed;
int interactive = FALSE;

/* I didn't know how to call this struct, but I think desk fits
   nicely, since I intend to load all the relevant information I have
   about the current working row/column/square into this struct
   (putting it on the desk).  Then I do all the work like removing
   possible numbers, putting uniquely defined numbers in the right
   places and so on.  After that I put something else on the desk to
   work on. */

static struct desk
{
  /* fields contains pointers to row, column or subsquare */
  struct f **fields;

  /* index contains index of row, column or subsquare */
  int index;

} desktop;

/* Determine the hamming-weight of number.  The hamming weight of a
   number is the the number of ones that are needed to write the
   number down in binary notation. Example:
   5 in binary notation is 101, so popcount(5) = 3 */
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

/* ns are the possible numbers that can go into a field.
   Since ns has 0s and 1s in the corresponding places, I was able to
   use an ugly one-liner.
   The function returns true if ns contains n, else false. */
static int contains(int ns, int n)
{
  return ( (ns & (1 << (n - 1))) > 0 ? TRUE : FALSE);
}

/* loads the row k to the desktop.  So int the desktop-struct, the
   row can be worked on as if a normal array.  This is not so
   interesting in case of rows or cols, but more in the function
   load_squ below*/
static void load_row(struct f **fp)
{
  desktop.fields = fp;
}

void reverse(char *s)
{
  int c, i, j;
  for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
    }
}

void itoa(int n, char *s)
{
  int i, sign;
  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

/* Removes number n from the row that is currently loaded. */
static void rem_n_tmp(int n)
{
  int i;
  char row[9];
  for (i = 0; i < 9; i++)
    {
      itoa(desktop.fields[i]->n, row + i);
    }

  debug("remove %d from row %s", n, row);

  for (i = 0; i < 9; i++)
    {
      if ( contains(desktop.fields[i]->ns, n) )
        {
          desktop.fields[i]->ns &= ~(1 << (n - 1));
          changed = TRUE;
        }
    }
}

/* Convert the index (i,j) to a the corresponding number of a square */
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

/* Removes set numbers at columns, rows and squares */
static void rem_n_at(struct s *sp, struct f *fp)
{
  load_row(sp->normal + 9 * fp->row_i);
  rem_n_tmp(fp->n);
  load_row(sp->transposed + 9 * fp->col_j);
  rem_n_tmp(fp->n);
  load_row(sp->transformed + 9 * get_squ_number(fp->row_i, fp->col_j));
  rem_n_tmp(fp->n);
}

/* Check each entry of the sudoku.  If it is non-zero, do all the
   according removals with set_n_at */
static void init_ns(struct s *sp)
{
  debug("Initializing ns");
  if (interactive)
    {
      log_info("%s", "Initializing.  Press [Enter] to continue");
      getchar();
    }

  int i, j;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
        {
          if (sp->normal[9 * i + j]->n != 0)
            {
              rem_n_at(sp, sp->normal[9 * i + j]);
            }
        }
    }
  if (interactive) printer_cli(sp);
}

/* Set index (i,j) to number n.  Do all the easy removals in the
   corresponding row/col/squ, and set ns to 0 */
static void set_n_at(struct s *sp, struct f *fp, int n)
{
  debug("Set %d at (%d,%d)", n, fp->row_i, fp->col_j);
  if (interactive)
    {
      log_info("setting %d at (%d,%d)", n, fp->row_i, fp->col_j);
      log_info("%s", "Press [Enter] to continue.");
      getchar();
    }
  if (fp->n != n){
    fp->n = n;
    fp->ns = 0;
    rem_n_at(sp, fp);
    changed = TRUE;
  }
  if (interactive)
    {
      printer_cli(sp);
    }
}

/* If there is only one possibilty left for a number, set this
   number.  This means if popcount(ns) == 1, set the corresponding
   number. */
static void set_single(struct s *sp, struct f *fp)
{
  debug("%s(%d)[%d]\n", "set_single for field containing", fp->n, fp->ns);
  int k;
  for (k = 0; k < 9; k++)
    {
      if ((1 << k) == fp->ns)
        {
          set_n_at(sp, fp, k + 1);
        }
    }
}

/* check for fields, where only one possible number is left, and set
   those fields. */
static void set_singles(struct s *sp)
{
  int i,j;
  if (interactive)
    {
      log_info("%s", "Setting singles.  Press [Enter] to continue");
      getchar();
    }
  debug("%s", "Set all singles");

  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
        {
          if (popcount(sp->normal[9 * i + j]->ns) == 1)
            {
              set_single(sp, sp->normal[9 * i + j]);
	    }
        }
    }
  debug("%s", "All singles set");
}

static void remove_tuple(int tupel)
{
  int i;
  for (i = 0; i < 9; i++)
    {
      if ( desktop.fields[i]->ns != tupel )
	{
	  desktop.fields[i]->ns &= (511 ^ tupel);
	}
    }
}

static void check_row_for_tuples(int i)
{
  int tuple, tmp_ns, c, j;
  struct f *tmp_fp[9];
  for (tuple = 1; tuple < 9; tuple++)
    {
      debug("looking for tuples of length %d", tuple);
      tmp_ns = c = 0;
      for (j = 0; j < 9; j++)
	{
	  if ( (c == 0) && ( popcount(desktop.fields[j]->ns) == tuple ))
	    {
	      tmp_ns = desktop.fields[j]->ns;
	      tmp_fp[c++] = desktop.fields[j];
	      debug("found candidate for tuple of length %d at (%d,%d)", tuple, desktop.fields[j]->row_i, desktop.fields[j]->col_j);
	    }
	  else
	    {
	      if ( (popcount(desktop.fields[j]->ns) == tuple) && ( (tmp_ns ^ desktop.fields[j]->ns) == 0) )
		{
		  tmp_fp[c++] = desktop.fields[j];
		  debug("Tuple of length %d continues at (%d,%d)", tuple, tmp_fp[c - 1]->row_i, tmp_fp[c - 1]->col_j);
		}
	    }
	}
      if ( c == tuple )
	{
	  remove_tuple(tmp_fp[0]->ns);
	  debug("Tuple in row %d, starting at (%d,%d)!", i, tmp_fp[0]->row_i, tmp_fp[0]->col_j);
	}
    }
}

static void remove_tuples(struct s *sp)
{
  int i;

  for (i = 0; i < 9; i++)
    {
      debug("looking for tuples in row %d", i);

      load_row(sp->normal + 9 * i);
      check_row_for_tuples(i);
      load_row(sp->transposed + 9 * i);
      check_row_for_tuples(i);
      load_row(sp->transformed + 9 * i);
      check_row_for_tuples(i);
    }
}

/* Assume that find_uniq_tmp below found a number.  Set this number. */
static void set_uniq_tmp(struct s *sp, int n)
{
  /* printf("%s\n", "set_uniq_tmp"); */
  int i;
  for (i = 0; i < 9; i++)
    {
      if ( contains(desktop.fields[i]->ns, n) )
        {
	  set_n_at(sp, desktop.fields[i], n);
        }
    }
}

/* Check the current desk, if there is a number, that can be set only
   in one place.  If yes, set this number there.  Artificially
   constructed example:
   +-----------+
   |000|100|000|
   |000|000|100|
   |200|000|000|
   +-----------+
   |000|010|000|
   |000|000|010|
   |300|000|000|
   +-----------+
   |000|001|000|
   |000|000|001| 1 goes in the bottem-left corner, since this is the
   |x00|000|000| only place, where 1 is possible.
   +~~~~~~~~~~~~ */
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
          set_uniq_tmp(sp, n + 1);
        }
    }
}

/* Find uniqs in rows/col/squs.  Set them. */
static void set_uniqs(struct s *sp)
{
  if (interactive)
    {
      log_info("%s", "Setting uniqs.  Press [Enter] to continue");
      getchar();
    }
  debug("%s", "Set all uniqs");
  int k;
  for (k = 0; k < 9; k++)
    {
      load_row(sp->normal + 9 * k);
      find_uniq_tmp(sp);
      load_row(sp->transposed + 9 * k);
      find_uniq_tmp(sp);
      load_row(sp->transformed + 9 * k);
      find_uniq_tmp(sp);
    }
  debug("%s", "All uniqs set");
}

/* I call this function find_shadows.  I have no better idea.  Since
   the name is rather bad, I better explain what I want it to do:
   Assume you have the following sudoku:
   +-----------+
   |7_3|x9x|4_8|  Counting from 0, we look at square 1.  Number 3
   |4__|8x7|___|  cannot go where I put x.  So it has to go to the
   |98_|_4_|__7|  bottom, left or right to 4.  This means, that we
   +-----------+  can eliminate 3 from the bottom line in square 2.
   |14_|___|_6_|
   |3_8|___|7_2|  We will plug each triple we have into and.  If the
   |67_|___|_1_|  result is greater than 0, we plug the result into
   +-----------+  xor with the remaining numbers.  If the result is
   |5__|_3_|__6|  greater than 0, we have some numbers we can
   |83_|2_9|___|  eliminate.  All we have to do is remembering the
   |2_4|_5_|3_9|  indices.
   +-----------+
*/
static void find_shadows(struct s *sp, int mode)
{
  debug("%s", "find_shadows");
  int tripel; /* specify at which tripel you are working */
  int candidates; /* holds all the numbers that can go into the
                     current tripel */
  int rest; /* holds the rest of the current 9-tupel to verify the
               candidates.*/
  int i,row;

  if (interactive)
    {
      log_info("%s", "Finding shadows:");
      log_info("%s", "Press [Enter] to continue.");
      getchar();
    }

  for (row = 0; row < 9; row++)
    {
      for (tripel = 0; tripel < 9; tripel += 3)
        {
	  switch ( mode )
	    {
	    case 1:
	      load_row(sp->normal + 9 * row);
	      break;
	    case 2:
	      load_row(sp->transposed + 9 * row);
	      break;
	    case 3:
	      load_row(sp->transformed + 9 * row);
	      break;
	    case 4:
	      load_row(sp->transforposed + 9 * row);
	      break;
	    }
          candidates = (desktop.fields[tripel]->ns |
                        desktop.fields[tripel + 1]->ns |
                        desktop.fields[tripel + 2]->ns);
          rest = 0;
	  switch ( mode )
	    {
	    case 1:
	      load_row(sp->transformed + 9 * get_squ_number(row,tripel));
	      break;
	    case 2:
	      load_row(sp->transforposed + 9 * get_squ_number(row, tripel));
	      break;
	    case 3:
	      load_row(sp->normal + 9 * get_squ_number(row, tripel));
	      break;
	    case 4:
	      load_row(sp->transposed + 9 * get_squ_number(row, tripel));
	      break;
	    default: log_err("I do not know what to do with %d", mode);
	    }

          for (i = 0; i < 6; i++)
            {
              if (row % 3 == 0){
                rest = rest | desktop.fields[i + 3]->ns;
              }  else if (row % 3 == 1 )
                {
                  if (i < 3) rest = rest | desktop.fields[i]->ns;
                  if (i >= 3 ) rest = rest | desktop.fields[i + 3]->ns;
                } else if (row % 3 == 2)
                {
                  rest = rest | desktop.fields[i]->ns;
                }
            }
          candidates = (511 ^ rest) & candidates;

	  switch ( mode )
	    {
	    case 1:
	      load_row(sp->normal + 9 * row);
	      break;
	    case 2:
	      load_row(sp->transposed + 9 * row);
	      break;
	    case 3:
	      load_row(sp->transformed + 9 * row);
	      break;
	    case 4:
	      load_row(sp->transforposed + 9 * row);
	      break;
	    default: log_err("I do not know what to do with %d", mode);
	    }

          for (i = 0; i < 6; i++)
            {
              if (tripel / 3 == 0)
                {
                  if ( desktop.fields[i + 3]->ns !=
                       (desktop.fields[i + 3]->ns & (~candidates)) )
                    {
                      desktop.fields[i + 3]->ns =
                        desktop.fields[i + 3]->ns & (~candidates);
                      changed = TRUE;
                    }
                }  else if (tripel / 3 == 1 )
                {
                  if (i < 3)
                    {
                      if ( desktop.fields[i]->ns !=
                           (desktop.fields[i]->ns & (~candidates) ))
                        {
                          desktop.fields[i]->ns = desktop.fields[i]->ns & (~candidates);
                          changed = TRUE;
                        }
                    }
                  if (i >= 3 )
                    {
                      if ( desktop.fields[i + 3]->ns !=
                           (desktop.fields[i + 3]->ns & (~candidates)))
                        {
                          desktop.fields[i + 3]->ns =
                            desktop.fields[i + 3]->ns & (~candidates);
                          changed = TRUE;
                        }
                    } else if (tripel / 3 == 2)
                    {
                      if ( desktop.fields[i]->ns !=
                           (desktop.fields[i]->ns & (~candidates)) )
                        {
                          desktop.fields[i]->ns = desktop.fields[i]->ns & (~candidates);
                          changed = TRUE;
                        }
                    }
                }
            }
        }
    }

  if (interactive)
    {
      printer_cli(sp);
    }
}

/* check if the fields f and g are equal */
static int f_equal(struct f *f, struct f *g)
{
  if ( (f->row_i == g->row_i) && (f->col_j == g->col_j) )
    {
      return TRUE;
    }
  else return FALSE;
}

/* check if field f can be seen from field g */
static int f_visible(struct f *f, struct f *g)
{
  if ( (f->row_i == g->row_i) ||
       (f->col_j == g->col_j) ||
       (get_squ_number(f->row_i, f->col_j) ==
	get_squ_number(g->row_i, g->col_j)))
    {
      return TRUE;
    }
  else return FALSE;

}

/* Check if candidate is a promising start for a golden chain.
 * candidate needs to have a popcount of 2. It has to see another
 * field which has a popcount of 2 and shares exactly one number with
 * the candidate. These checks are done for fields currently loaded
 * on the desktop. */
static int check_golden_candidate_helper(struct f *candidate)
{
  int i;
  for (i = 0; i < 9; i++)
    {
      if ( popcount(desktop.fields[i]->ns) == 2 )
	{
	  if ( ! (f_equal(desktop.fields[i], candidate)) )
	    {
	      if ( popcount(desktop.fields[i]->ns | candidate->ns) == 3)
		{
		  debug("found a candidate at (%d,%d)!",
			   desktop.fields[i]->row_i,
			   desktop.fields[i]->col_j);

		  return TRUE;
		}
	    }
	}
    }
  return FALSE;
}

/* Check if candidate for starting a golden chain can see something
   useful */
static int check_golden_candidate(struct s *sp, struct f *candidate)
{
  int row_check, col_check, squ_check;
  row_check = col_check = squ_check = 0;
  debug("check candidate at (%d,%d)",
	candidate->row_i, candidate->col_j);
  load_row(sp->normal + 9 * candidate->row_i);
  row_check = check_golden_candidate_helper(candidate);
  load_row(sp->transposed + 9 * candidate->col_j);
  col_check = check_golden_candidate_helper(candidate);
  load_row(sp->normal + 9 * get_squ_number(candidate->row_i,candidate->col_j));
  squ_check = check_golden_candidate_helper(candidate);
  debug("check = %d", (row_check | col_check | squ_check));
  return (row_check | col_check | squ_check);
}

/* Check if next is a valid field for a golden chain.  It should
   perform the same checks as check_golden_candidate, but
   adittionally validate whether next is already contained int the
   chain. */
static int check_chain_link(struct f *current, struct f *next)
{
  if ( (popcount(current->ns) == 2) && (popcount(next->ns) == 2) )
    {
      if ( popcount(current->ns | next->ns) == 3 )
	{
	  if ( f_visible(current, next) && (! f_equal(current, next)) )
	    {
	      return TRUE;
	    }
	}
    }
  return FALSE;
}

/* find fields, that can be seen from both fields.
 * The intersection can contain at most 15 fields, -2 for the fields
 * themselves. */
static struct f *find_intersection(struct f *first, struct f *last)
{
  if ( f_equal(first, last) )
    {
      log_err("The same field was passed");
      return first;
    }
  /* both are in the same row */
  if (first->row_i == last->row_i);
  /* both are in the same column */
  if (first->col_j == last->col_j);
  /* both are in the same square */
  if ( get_squ_number(first->row_i, first->col_j)
       == get_squ_number(last->row_i, last->col_j) );
  return first;
}

static void start_golden_chain(struct s *sp)
{
  int i, j;
  struct f *acc[81];
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j ++)
	{
	  if ( check_golden_candidate(sp, sp->normal[9 * i + j]) )
	    {
	      acc[0] = sp->normal[9 * i + j];
	      build_golden_chain(acc);
	    }
	}
    }
}

/*  */
static int build_golden_chain(struct f **acc)
{
  return FALSE;
}

/* test sum and product of rows */
static int test(struct s *sp)
{
  int i,j,p,s,c,ret;
  c = 0;
  for (i = 0; i < 9; i++)
    {
      s = 0;
      p = 1;
      for (j = 0; j < 9; j++)
        {
          s += sp->normal[9 * i + j]->n;
          p *= sp->normal[9 * i + j]->n;
        }
      if (p == 362880 && s == 45) c++;
      if ((p != 362880) && (p > 0)) ret = 2;
    }
  if (ret == 2)
    {
      return ret;
    }
  else return (c == 9 ? 0 : 1);
}

int solver(struct s *sp, int inter)
{
  int i;
  changed = TRUE;
  interactive = inter;
  init_ns(sp);
  do
    {
      changed = FALSE;
      set_singles(sp);
      set_uniqs(sp);
      remove_tuples(sp);
      for (i = 1; i < 5; i++)
	{
	  find_shadows(sp, i); /* find shadows in normal sudoku */
	}
    }
  while (changed);
  /* find_golden_chain_start(sp); */
  return test(sp);
}
