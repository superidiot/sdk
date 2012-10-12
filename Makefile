objects = main.o printer.o reader.o

sudoku : $(objects)
	cc -o sudoku $(objects)

main.o : main.c sudoku.h
reader.o : reader.c sudoku.h
printer.o : printer.c sudoku.h

.PHONY : clean
clean : 
	rm sudoku $(objects)
