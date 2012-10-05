#include <stdio.h>

#include "sudoku.h"

void printer_cli (struct s *sp){
  int i,j;
  for (i = 0; i < 9; i++){
    if (i % 3 == 0){
      printf("%s","+-----------+\n");
    }
    for (j = 0; j < 9; j++){
      if (j % 3 == 0){
	printf("%s","|");
      }
      if (sp->a[i][j].n != 0){
	printf("%1d", sp->a[i][j].n);
      }
      else {
	printf("%s", "_");
      }
    }
    printf("%1s","|\n");
  }
  printf("%s","+-----------+\n");
  
  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      int k;
      int tmp;
      for (k = 0; k < 10; k++){
	tmp = sp->a[i][j].ns[k];
	if (tmp == 0)
	  printf("%s", "_");
	else
	  printf("%1d",tmp);
      }
      printf("%s"," ");
    }
    printf("%s","\n");
  }
}

void printer_ps (struct s *sp, const char *filename){
  int i,j,k;
  FILE *psfile = fopen(filename, "w");
  fprintf(psfile, "%s", "/cm {28.34 mul} def /squCenter {0.2 cm 0.2 cm rmoveto} def /fontGr {20} def /gr {1} def /showNumber {dup stringwidth pop gr cm sub 2 div neg gr cm fontGr 4 mul 5 div sub 2 div rmoveto show} def newpath gsave 2 setlinewidth 1 cm 1 cm moveto 9 cm 0 cm rlineto 0 cm 9 cm rlineto -9 cm 0 cm rlineto 0 cm -9 cm rlineto closepath stroke grestore 1 cm 2 cm moveto 9 cm 0 cm rlineto 1 cm 3 cm moveto 9 cm 0 cm rlineto stroke gsave 2 setlinewidth 1 cm 4 cm moveto 9 cm 0 cm rlineto stroke grestore 1 cm 5 cm moveto 9 cm 0 cm rlineto 1 cm 6 cm moveto 9 cm 0 cm rlineto stroke gsave 2 setlinewidth 1 cm 7 cm moveto 9 cm 0 cm rlineto stroke grestore 1 cm 8 cm moveto 9 cm 0 cm rlineto 1 cm 9 cm moveto 9 cm 0 cm rlineto stroke 2 cm 1 cm moveto 0 cm 9 cm rlineto 3 cm 1 cm moveto 0 cm 9 cm rlineto stroke gsave 2 setlinewidth 4 cm 1 cm moveto 0 cm 9 cm rlineto stroke grestore 5 cm 1 cm moveto 0 cm 9 cm rlineto 6 cm 1 cm moveto 0 cm 9 cm rlineto stroke gsave 2 setlinewidth 7 cm 1 cm moveto 0 cm 9 cm rlineto stroke grestore 8 cm 1 cm moveto 0 cm 9 cm rlineto 9 cm 1 cm moveto 0 cm 9 cm rlineto 10 cm 1 cm moveto stroke\n /Helvetica-Bold findfont fontGr scalefont setfont ");
  for (j = 8; j >= 0; j--){
    for (i = 0; i < 9; i++){
      if (sp->a[i][j].n != 0){
	fprintf(psfile, "%d cm %d cm moveto (%d) showNumber ", (j + 1), (9 - i), sp->a[i][j].n);
      } else {
	fprintf(psfile, "gsave /Helvetica findfont 5 scalefont setfont %d cm %d cm moveto 0 20 rmoveto (", (j + 1), (9 - i));
	for (k = 1; k < 10; k++){
	  if ( k < 3 ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d  ", k);
	    } else {
	      fprintf(psfile, "%s", "   ");
	    }
	  } else if ( k == 3 ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show %d cm %d cm moveto 0 13 rmoveto (", k, (j + 1), (9 - i));
	    } else {
	      fprintf(psfile, " ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show %d cm %d cm moveto 0 13 rmoveto (", (j + 1), (9 - i));
	    }
	  } else if ( (k >= 2) && (k < 6) ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d  ", k);
	    } else {
	      fprintf(psfile, "%s", "   ");
	    }
	  } else if ( k == 6 ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show %d cm %d cm 6 add moveto (", k, (j + 1), (9 - i));
	    } else {
	      fprintf(psfile, " ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show %d cm %d cm 6 add moveto (", (j + 1), (9 - i));
	    }
	  } else if ( (k >= 5) && (k < 9) ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d  ", k);
	    } else {
	      fprintf(psfile, "%s", "   ");
	    }
	  } else if ( k == 9 ){
	    if (sp->a[i][j].ns[k] != 0){
	      fprintf(psfile, "%d) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show grestore ", k);
	    } else {
	      fprintf(psfile, "%s", " ) dup stringwidth pop gr cm sub 2 div neg 0 rmoveto show grestore ");
	    }
	  }	    
	}
      }
    }
  }
  fclose(psfile);
}
