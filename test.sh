#!/bin/sh
for i in `ls sudokus`; do
    ./sudoku -f sudokus/$i > /dev/null;
    RET=$?;
    if [ 0 -eq $RET ]; then
	echo "\033[1;32m solved sudoku $i";
    elif [ 1 -eq $RET ]; then
	echo "\033[1;30m could not solve $i";
    else
	echo "\033[1;31m made an error solving $i";
    fi;
done;
