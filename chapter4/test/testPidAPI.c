#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include "pidAPI.h"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        exit(-1);
    }
    int numThreads = atoi(argv[1]);
    if (numThreads < 0)
    {
        fprintf(stderr, "Invalid number of threads. Number must be positive\n");
        exit(-1);
    }
    if (allocate_map() == -1)
    {
        fprintf(stderr, "Error allocating map\n");
        exit(-1);
    }
    omp_set_num_threads(numThreads);
    #pragma omp parallel for
    for (int i = 0; i < numThreads; i++)
    {
        int pid = allocate_pid();
        if (pid == -1)
        {
            fprintf(stderr, "Error allocating pid\n");
            exit(-1);
        }
        printf("Thread %d: Allocated pid %d\n", i, pid);
        int duration = rand() % 10;
        sleep(duration);
        release_pid(pid);
        printf("Thread %d: Released pid %d after %d\n", i, pid, duration);
    }
}