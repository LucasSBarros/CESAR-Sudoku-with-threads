# Sudoku-with-threads

This program is a simple Sudoku validator that checks if a given Sudoku solution is valid or not. The program takes a file as input that contains the Sudoku solution, and it prints whether it is a valid solution or not.

How to use

1. Compile the program using the following command:

gcc sudoku.c -o sudoku 

or just simple use the command: 

make 

2. Run the program using the following command:

./sudoku <input_file>

Replace <input_file> with the path of the file that contains the Sudoku solution.

Input format

The input file must contain the Sudoku solution in the following format:

1. The first line must contain the size of the Sudoku grid in the following format:                <number_of_rows>x<number_of_columns>.
2. The second line must contain the size of the subgrid in the following format: <number_of_rows>x<number_of_columns>.
3. The remaining lines must contain the Sudoku solution. Each line must contain one row of the Sudoku grid, and the values must be separated by spaces.

Output format

If the Sudoku solution is valid, the program will print on an output file the following message:
SUCESS

If the Sudoku solution is not valid, the program will print on an output file the following message:
FAIL

Implementation details

The program reads the input file and creates a Sudoku struct that contains the Sudoku grid and other relevant information. Then, it creates three threads that validate the rows, columns, and subgrids of the Sudoku solution concurrently. If any of the threads detects an invalid row, column, or subgrid, the program prints an error message and terminates. If all threads complete successfully, the program prints a success message.

Note that the program assumes that the input file is well-formed and that it does not perform any error checking beyond checking for empty files and files that end in spaces or newlines.
