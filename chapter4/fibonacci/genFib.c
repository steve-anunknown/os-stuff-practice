#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long unsigned int *sequence;

void *genFibs(void* param)
{
    long unsigned int i;
    long unsigned int terms = *(long unsigned int*)param;
    if (terms > 0)
    {
        sequence[0] = 0;
    }
    if (terms > 1)
    {
        sequence[1] = 1;
    }
    for (i = 2; i < terms; i++)
    {
        sequence[i] = sequence[i-1] + sequence[i-2];
    }
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./genFib <number of terms>\n");
        exit(-1);
    }
    int terms = atoi(argv[1]);
    if (terms < 1)
    {
        fprintf(stderr, "Number of terms must be positive\n");
        exit(-1);
    }
    sequence = (long unsigned int *)malloc(terms * sizeof(long unsigned int));
    if (sequence == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        exit(-1);
    }

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, genFibs, &terms);
    pthread_join(tid, NULL);

    for (int i = 0; i < terms; i++)
    {
        printf("%lu ", sequence[i]);
    }
    printf("\n");
    free(sequence);
    return 0;
}