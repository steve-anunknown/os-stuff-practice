#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "barrier_utils.h"

void *test(void *param)
{
    printf("BEFORE\n");
    barrier_point();
    printf("PASSED\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int NUM_THREADS = atoi(argv[1]);
    if (NUM_THREADS < 1)
    {
        fprintf(stderr, "num_threads must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
    pthread_t threads[NUM_THREADS];
    int i;
    int ret;
    int n = NUM_THREADS;
    printf("=====================================================================\n");
    printf("|| NO 'PASSED' MESSAGE SHOULD BE PRINTED BEFORE A 'BEFORE' MESSAGE ||\n");
    printf("=====================================================================\n");
    init(n);
    for (i = 0; i < NUM_THREADS; i++)
    {
        ret = pthread_create(&threads[i], NULL, test, NULL);
        if (ret)
        {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}