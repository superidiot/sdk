struct f {
  int n;
  int ns[10];
};

struct s {
  struct f a[9][9];
} sudoku;

void reader(FILE *fd, struct s *sp);
void printer_cli (struct s *sp);
void printer_ps (struct s *sp, const char *filename);
void solver (struct s *sp);
