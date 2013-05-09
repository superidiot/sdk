/* http://en.wikipedia.org/wiki/Row-major_order */
/* offset = row*NUMCOLS + column. */
#define MATRIX_ROW_MAJOR_IDX(A, ncol, row, col) A[row * ncol + col]
/* offset = row + column*NUMROWS. */
#define MATRIX_COL_MAJOR_IDX(A, nrow, row, col) A[row + col * nrow]

#define TRUE 1
#define FALSE 0


struct f {
  int n;
  int ns;
  int row_i;
  int col_j;
};

struct s {
  struct f *normal[81];
  struct f *transposed[81];
  struct f *transformed[81];
  struct f *transforposed[81];
};

struct s sudoku;

enum numbers { ONE   = 1,
	       TWO   = 1 << 1,
	       THREE = 1 << 2,
	       FOUR  = 1 << 3,
	       FIVE  = 1 << 4,
	       SIX   = 1 << 5,
	       SEVEN = 1 << 6,
	       EIGHT = 1 << 7,
	       NINE  = 1 << 8 };

struct s *reader(FILE *fd);

void printer_cli (struct s *sp);
void printer_ps (struct s *sp,
		 const char *filename,
		 int offsetX,
		 int offsetY,
		 const char *mode);

int solver (struct s *sp, int interactive);
