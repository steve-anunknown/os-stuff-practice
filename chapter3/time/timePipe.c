/*
    A C program that determines the amount of
    time necessary to run a COMMAND from the
    COMMAND line. It will be run as follows:
    ./time <COMMAND>
    and will report the amount of elapsed
    time to run the specified COMMAND.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./time <COMMAND>\n");
        exit(1);
    }

    int fd[2];
    pid_t pid;
    struct timeval start, end;
    double elapsed;
    char* COMMAND = argv[1];
    char commandPath[30] = "/usr/bin/";
    strcat(commandPath, COMMAND);
    // construct pipe
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed.\n");
        exit(1);
    }
    // pipe is ready
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // child process
        close(fd[READ_END]);
        gettimeofday(&start, NULL);
        write(fd[WRITE_END], &start, sizeof(start));
        close(fd[WRITE_END]);
        execlp(commandPath, COMMAND, (char *) NULL);
        fprintf(stderr, "Exec failed.\n");
        exit(1);
    }
    else
    {
        // parent process
        close(fd[WRITE_END]);
        wait(NULL);
        gettimeofday(&end, NULL);
        read(fd[READ_END], &start, sizeof(start));
        close(fd[READ_END]);
        elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);
        printf("Elapsed time: %f seconds.\n", elapsed);
    }
}