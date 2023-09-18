#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>

void colatzSequence(int n, int *mem_pointer)
{
    int index = 0;
    while (n != 1)
    {
        mem_pointer[index ++] = n;
        n = (n % 2 == 0) ? (n / 2) : (3 * n + 1); 
    }
    mem_pointer[index ++] = n;
}

int usageCheck(int argc, char *argv[])
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
    return input;
}


int main(int argc, char *argvp[])
{
    int input = usageCheck(argc, argvp);
    
    // create shared memory
    int fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, 4096);
    int *mem_pointer = (int *) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem_pointer == MAP_FAILED)
    {
        fprintf(stderr, "Map failed\n");
        return -1;
    }

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
        colatzSequence(input, mem_pointer);
    }
    else
    {
        // parent process
        wait(NULL);
        int index = 0;
        while (mem_pointer[index] != 1)
        {
            printf("%d ", mem_pointer[index ++]);
        }
        printf("%d\n", mem_pointer[index]);
        shm_unlink("OS");        
    }

    return 0;
}