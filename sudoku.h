struct f {
  int n;
  int ns[10];
};

struct s {
  struct f a[9][9];
} sudoku;

void reader(FILE *fd, struct s *sp);
void printer (struct s *sp);
void solver (struct s *sp);
