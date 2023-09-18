/*
    A C program that determines the amount of
    time necessary to run a command from the
    command line. It will be run as follows:
    ./time <command>
    and will report the amount of elapsed
    time to run the specified command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    // usage check
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./time <command>\n");
        exit(1);
    }

    // variables
    struct timeval start, end;
    double elapsed;

    char* command = argv[1];
    char commandPath[30] = "/usr/bin/";
    strcat(commandPath, command);

    int fd;
    const int SIZE = sizeof(struct timeval);
    const char *NAME = "OS";
    char *mem_pointer = NULL;

    // create shared memory
    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    mem_pointer = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem_pointer == MAP_FAILED)
    {
        printf("Map failed\n");
        return -1;
    }
    // shared memory is ready

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
        gettimeofday(&start, NULL);
        sprintf(mem_pointer, "%ld %ld", start.tv_sec, start.tv_usec);
        execlp(commandPath, command, (char *) NULL);
        fprintf(stderr, "Exec failed.\n");
        exit(1);
    }
    else
    {
        // parent processs
        wait(NULL);
        gettimeofday(&end, NULL);
        sscanf(mem_pointer, "%ld %ld", &start.tv_sec, &start.tv_usec);
        elapsed = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);
        printf("Elapsed time: %f seconds\n", elapsed);
        shm_unlink(NAME);
    }

    return 0;

}