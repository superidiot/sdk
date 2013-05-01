#include <stdio.h>
#include "sudoku.h"

static void print_bin(int ns)
{
  int bin = 256;
  while (bin > 0)
    {
      if ((ns & bin) == bin)
	{
	  printf("%d", 1);
	  ns &= (~bin);
	  bin >>= 1;
	}
      else {
	printf("%d",0);
	bin >>= 1;
      }
    }
}

void printer_cli (struct s *sp)
{
  int i,j;
  for (i = 0; i < 9; i++)
    {
      if (i % 3 == 0)
	{
	  printf("%s","+-----------+\n");
	}
      for (j = 0; j < 9; j++)
	{
	  if (j % 3 == 0)
	    {
	      printf("%s","|");
	    }
	  if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->n != 0)
	    {
	      printf("%1d", MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->n);
	    }
	  else
	    {
	      printf("%s", "_");
	    }
	}
      printf("%1s","|\n");
    }
  printf("%s","+-----------+\n");

  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  int k;
	  int tmp;
	  tmp = MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns;
	  if (tmp == 0)
	    {
	      printf("%s", "_________");
	    }
	  else
	    {
	      print_bin(tmp);
	    }
	  printf("%s"," ");
	}
      printf("%s","\n");
    }
}

void printer_ps (
		 struct s *sp,
		 const char *filename,
		 int offsetX,
		 int offsetY,
		 const char *mode)
{
  int i,j,k;
  FILE *psfile = fopen(filename, mode);

  fprintf(psfile, "/cm {28.34 mul} def /fontGr {20} def /gr {1} def /showNumber {dup stringwidth pop gr cm sub 2 div neg gr cm fontGr 4 mul 5 div sub 2 div rmoveto show} def /offset {%d cm %d cm} def ", offsetX, offsetY);
  fprintf(psfile, "%s", "newpath gsave 2 setlinewidth offset moveto 9 cm 0 cm rlineto 0 cm 9 cm rlineto -9 cm 0 cm rlineto 0 cm -9 cm rlineto closepath stroke 3 3 6 { offset moveto cm 0 rmoveto 0 9 cm rlineto stroke } for 3 3 6 { offset moveto cm 0 exch rmoveto 9 cm 0 rlineto stroke } for grestore offset moveto 1 1 9 { dup cm 0 rmoveto 0 cm 9 cm rlineto neg cm -9 cm rmoveto } for offset moveto 1 1 9 { dup cm 0 exch rmoveto 9 cm 0 cm rlineto neg cm -9 cm exch rmoveto } for stroke\n");
  fprintf(psfile, "%s", "/Helvetica-Bold findfont fontGr scalefont setfont\noffset moveto\n");
  for (j = 8; j >= 0; j--)
    {
      for (i = 0; i < 9; i++)
	{
	  if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->n != 0)
	    {
	      fprintf(psfile, "offset moveto %d cm %d cm rmoveto (%d) showNumber\n", j, (8 - i), MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->n);
	    } else
	    {
	      fprintf(psfile, "gsave /Helvetica findfont 5 scalefont setfont offset moveto %d cm %d cm rmoveto 0 20 rmoveto (", j, (8 - i));
	      for (k = 1; k < 10; k++)
		{
		  if ( k < 3 )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d  ", k);
			} else
			{
			  fprintf(psfile, "%s", "    ");
			}
		    } else if ( k == 3 )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show offset moveto %d cm %d cm rmoveto 0 13 rmoveto (", k, j, (8 - i));
			} else
			{
			  fprintf(psfile, "  ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show offset moveto %d cm %d cm rmoveto 0 13 rmoveto (", j, (8 - i));
			}
		    } else if ( (k >= 2) && (k < 6) )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d  ", k);
			} else
			{
			  fprintf(psfile, "%s", "    ");
			}
		    }
		  else if ( k == 6 )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show offset moveto %d cm %d cm 6 add rmoveto (", k, j, (8 - i));
			} else
			{
			  fprintf(psfile, "  ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show offset moveto %d cm %d cm 6 add rmoveto (", j, (8 - i));
			}
		    } else if ( (k >= 5) && (k < 9) )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d  ", k);
			} else
			{
			  fprintf(psfile, "%s", "    ");
			}
		    } else if ( k == 9 )
		    {
		      if (MATRIX_ROW_MAJOR_IDX(sp->normal, 9, i, j)->ns != 0)
			{
			  fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show grestore\n", k);
			} else
			{
			  fprintf(psfile, "%s", "  ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show grestore\n");
			}
		    }
		}
	    }
	}
    }
  fclose(psfile);
}
