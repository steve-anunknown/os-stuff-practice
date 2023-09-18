/*
    C program that forks a child process
    that ultimately becomes a zombie process.
    This zombie process must remain in the
    system for at least 10 seconds.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("Child process created.\n");
        exit(0);
    }
    else
    {
        printf("Parent process created.\n");
        sleep(10);
        printf("Parent process exiting.\n");
        exit(0);
    }
}