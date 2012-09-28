struct f {
  int n;
  int ns[10];
  };

struct s {
  struct f a[9][9];
} sudoku;

void reader(FILE *fd);
void printer ();
void solver ();
