#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./piMonteCarloOpenMP <number of points>\n");
        exit(-1);
    }
    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "Invalid number of points. Number must be positive\n");
        exit(-1);
    }
    int points = atoi(argv[1]);
    int pointsInCircle = 0;
    double pi = 0;
    srand(time(NULL));

    #pragma omp parallel for
    for (int i = 0; i < points; i++)
    {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        if ((x * x + y * y) <= 1.0)
        {
            // this is ... critical :).
            // there is a race condition here.
            // the operation must be atomic,
            // otherwise the result will be wrong.
            #pragma omp critical
            pointsInCircle++;
        }
    }

    pi = 4.0 * ((double) pointsInCircle / points);
    printf("The value of pi is %f\n", pi);
    return 0;
}