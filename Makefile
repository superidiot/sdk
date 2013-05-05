CFLAGS=-Wall -g
objects = main.o printer.o reader.o solver.o

sudoku : $(objects)
	cc -o sudoku $(objects)

main.o : main.c sudoku.h
reader.o : reader.c sudoku.h
printer.o : printer.c sudoku.h
solver.o : solver.c sudoku.h

.PHONY : clean
clean : 
	rm sudoku sudoku.ps $(objects)
all: sudoku
