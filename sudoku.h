struct f {
  int n;
  int ns;
};

struct s {
  struct f normal[9][9];
  struct f transposed[9][9];
  struct f transformed[9][9];
  struct f transforposed[9][9];
};

struct s sudoku;

enum numbers { ONE = 1,
	       TWO = 1 << 1,
	       THREE = 1 << 2,
	       FOUR = 1 << 3,
	       FIVE = 1 << 4,
	       SIX = 1 << 5,
	       SEVEN = 1 << 6,
	       EIGHT = 1 << 7,
	       NINE = 1 << 8};

struct s *reader(FILE *fd);
 
void printer_cli (struct s *sp);

/* void printer_ps (struct s *sp, const char *filename, int offsetX, int offsetY, const char *mode); */
int solver (struct s *sp, int interactive);
