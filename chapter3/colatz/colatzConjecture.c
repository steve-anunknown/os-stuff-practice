#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void colatzSequence(int n)
{
    while (n != 1)
    {
        printf("%d ", n);
        if (n % 2 == 0)
        {
            n = n / 2;
        }
        else
        {
            n = 3 * n + 1;
        }
    }
    printf("%d\n", n);
}

void usageCheck(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./colatzConjecture <number>\n");
        exit(1);
    }
    int input = atoi(argv[1]);
    if (input <= 0)
    {
        fprintf(stderr, "Usage: ./colatzConjecture <number>\n");
        fprintf(stderr, "number must be positive.\n");
        exit(1);
    }
}


int main(int argc, char *argvp[])
{
    usageCheck(argc, argvp);

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // child process
        colatzSequence(input);
    }
    else
    {
        // parent process
        wait(NULL);
    }


    return 0;
}