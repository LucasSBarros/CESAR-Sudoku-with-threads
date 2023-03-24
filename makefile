all: file
file: sudoku.c
		gcc -o sudoku sudoku.c -lpthread
clean:
		rm -f sudoku