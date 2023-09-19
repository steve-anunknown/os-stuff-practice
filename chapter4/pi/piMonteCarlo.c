/*
    This is a program that uses the Monte Carlo method to estimate pi.
    The Monte Carlo method is a method of approximating the value of pi by
    randomly generating points on a 2D plane and calculating the ratio of
    points that fall within a circle inscribed in a square to the total
    number of points generated. The ratio is then multiplied by 4 to
    approximate pi.

    pi = 4 * (points in circle / total points)
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

double pi = 0;

void *monteCarlo(void* param)
{
    int totalPoints = *(int *) param;
    int pointsInCircle = 0;
    double x, y;

    srand(time(NULL));

    for (int i = 0; i < totalPoints; i++)
    {
        x = (double) rand() / RAND_MAX;
        y = (double) rand() / RAND_MAX;

        if (x * x + y * y <= 1) // same as using root
        {
            pointsInCircle++;
        }
    }

    pi = 4.0 * ((double) pointsInCircle / totalPoints);

    pthread_exit(0);
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./piMonteCarlo <number of points>\n");
        exit(-1);
    }
    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "Invalid number of points. Number must be positive\n");
        exit(-1);
    }
    int points = atoi(argv[1]);

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, monteCarlo, &points);
    pthread_join(tid, NULL);

    printf("The value of pi is %f\n", pi);
    return 0;
}