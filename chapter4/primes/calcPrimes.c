#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *printPrimes(void *param)
{
    int *n = (int *)param;
    // print all primes less than or equal to n
    for (int i = 2; i <= *n; i++)
    {
        int isPrime = 1;
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                isPrime = 0;
                break;
            }
        }
        if (isPrime)
        {
            printf("%d ", i);
        }
    }
    printf("\n");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./calcPrimes <integer value>\n");
        exit(-1);
    }
    int n = atoi(argv[1]);
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, printPrimes, &n);
    pthread_join(tid, NULL);
    return 0;

}