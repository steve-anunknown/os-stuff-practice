#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9

// Function to print the Sudoku grid
void printSudoku(int sudoku[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%2d ", sudoku[i][j]);
        }
        printf("\n");
    }
}

// Function to check if it's safe to place a number in a specific cell
int isSafe(int sudoku[N][N], int row, int col, int num) {
    // Check row and column
    for (int x = 0; x < N; x++) {
        if (sudoku[row][x] == num || sudoku[x][col] == num) {
            return 0;
        }
    }

    // Check 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }

    return 1;
}

// Function to solve Sudoku using backtracking
int solveSudoku(int sudoku[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (sudoku[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(sudoku, row, col, num)) {
                        sudoku[row][col] = num;
                        if (solveSudoku(sudoku)) {
                            return 1;
                        }
                        sudoku[row][col] = 0; // If placing num doesn't lead to a solution, backtrack
                    }
                }
                return 0; // If no number can be placed in this cell, the puzzle is unsolvable
            }
        }
    }
    return 1; // All cells filled, puzzle solved
}

// Function to generate a random valid Sudoku puzzle
void generateRandomSudoku(int sudoku[N][N]) {
    // Seed the random number generator
    srand(time(NULL));

    // Start with an empty Sudoku grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sudoku[i][j] = 0;
        }
    }

    // Solve the Sudoku puzzle
    solveSudoku(sudoku);

    // Randomly remove some numbers to create a puzzle
    int numToRemove = rand() % 50 + 40; // Remove between 40 to 89 numbers
    for (int i = 0; i < numToRemove; i++) {
        int row, col;
        do {
            row = rand() % N;
            col = rand() % N;
        } while (sudoku[row][col] == 0);
        sudoku[row][col] = 0;
    }
}

int main() {
    int sudoku[N][N];
    
    generateRandomSudoku(sudoku);

    // Print the Sudoku grid to a file
    FILE *file = fopen("sudoku.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%d ", sudoku[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Sudoku puzzle has been generated and saved to 'sudoku.txt'.\n");

    return 0;
}
