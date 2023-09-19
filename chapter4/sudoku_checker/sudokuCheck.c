
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#define SUDOKU_SIZE 9
int sudoku[SUDOKU_SIZE][SUDOKU_SIZE];

typedef struct
{
    int row;
    int col;
    int res;
} gridParams;

bool ROWS_OK = false;
void* checkRows(void* param)
{
    bool valid = true;
    for (int i = 0; i < SUDOKU_SIZE; ++i)
    {
        bool row[SUDOKU_SIZE] = {false};
        for (int j = 0; j < SUDOKU_SIZE; ++j)
        {
            if (row[sudoku[i][j] - 1])
            {
                valid = false;
                break;
            }
            row[sudoku[i][j] - 1] = true;
        }
    }
    // get thread id
    ROWS_OK = valid;
    pthread_exit(0);
}
bool COLS_OK = false;
void* checkCols(void* param)
{
    bool valid = true;
    for (int i = 0; i < SUDOKU_SIZE; ++i)
    {
        bool col[SUDOKU_SIZE] = {false};
        for (int j = 0; j < SUDOKU_SIZE; ++j)
        {
            if (col[sudoku[j][i] - 1])
            {
                valid = false;
                break;
            }
            col[sudoku[j][i] - 1] = true;
        }
    }
    COLS_OK = valid;
}
void* checkGrids(void* param)
{
    gridParams* p = (gridParams*)param;
    int row = p->row;
    int col = p->col;
    bool valid = true;
    bool grid[SUDOKU_SIZE] = {false};
    for (int i = row; i < row + 3; ++i)
    {
        for (int j = col; j < col + 3; ++j)
        {
            if (grid[sudoku[i][j] - 1])
            {
                valid = false;
                break;
            }
            grid[sudoku[i][j] - 1] = true;
        }
    }
    p->res = valid;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprinf(stderr, "Usage: ./sudokuCheck <file>\n");
        exit(-1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        exit(-1);
    }
    for (int i = 0; i < SUDOKU_SIZE; ++i)
    {
        for (int j = 0; j < SUDOKU_SIZE; ++j)
        {
            int n;
            if (read(fd, &n, sizeof(int)) != sizeof(int))
            {
                fprintf(stderr, "Error: Cannot read file %s\n", argv[1]);
                exit(-1);
            }
            sudoku[i][j] = n;
        }
    }
    close(fd);

    bool valid = true;
    pthread_t tid[11];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    gridParams  p[] = {{0, 0}, {0, 3}, {0, 6}, {3, 0}, {3, 3}, {3, 6}, {6, 0}, {6, 3}, {6, 6}};
    for (int i = 0; i < SUDOKU_SIZE; ++i)
    {
        pthread_create(&tid[i], &attr, checkGrids, &p[i]);
    }
    pthread_create(&tid[9], &attr, checkRows, NULL);
    pthread_create(&tid[10], &attr, checkCols, NULL);
    for (int i = 0; i < SUDOKU_SIZE; ++i)
    {
        pthread_join(tid[i], NULL);
        valid &= p[i].res;   
    }
    valid &= ROWS_OK;
    valid &= COLS_OK;
    if (valid)
    {
        printf("This is a valid sudoku.\n");
    }
    else
    {
        printf("This is not a valid sudoku.\n");
    }

    return 0;
}