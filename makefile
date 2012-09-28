sudoku : main.o printer.o reader.o
	cc -o sudoku main.o reader.o printer.o
main.o : sudoku.h
printer.o : printer.c sudoku.h
reader.o : reader.c sudoku.h
clean : 
	rm sudoku main.o reader.o printer.o
