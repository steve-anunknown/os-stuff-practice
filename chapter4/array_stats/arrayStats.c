/*
    multithreaded program that calculates various
    statistical values for an array of numbers.

    I believe this could be written in a bit more
    extensible of a way. I may get to it.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

float AVERAGE;
int MINIMUM;
int MAXIMUM;
int LENGTH;

void *calc_average(void *param)
{
    int *array = (int *)param;
    int sum = 0;
    for (int i = 0; i < LENGTH; i++)
    {
        sum += array[i];
    }
    AVERAGE = sum / LENGTH;
    pthread_exit(0);
}
void *calc_minimum(void *param)
{
    int *array = (int *)param;
    int min = array[0];
    for (int i = 1; i < LENGTH; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    MINIMUM = min;
    pthread_exit(0);
}
void *calc_maximum(void *param)
{
    int *array = (int *)param;
    int max = array[0];
    for (int i = 1; i < LENGTH; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    MAXIMUM = max;
    pthread_exit(0);
}

int isNumber(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Array with zero elements.\n");
        return 0;
    }
    /* check that the passed arguments are numbers */
    for (int i = 1; i < argc; i++)
    {
        if (!isNumber(argv[i]))
        {
            fprintf(stderr, "Invalid input.\n");
            fprintf(stderr, "Usage: arrayStats <int> <int> <int> ...\n");
            return 0;
        }
    }
    /* convert the arguments to integers */
    int* array = malloc(sizeof(int) * (argc - 1));
    LENGTH = argc - 1;
    for (int i = 1; i < argc; i++)
    {
        array[i - 1] = atoi(argv[i]);
    }
    /* create the threads */
    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, calc_average, array);
    pthread_create(&tid[1], &attr, calc_minimum, array);
    pthread_create(&tid[2], &attr, calc_maximum, array);
    /* wait for the threads to finish */
    for (int i = 0; i < 3; i++)
    {
        pthread_join(tid[i], NULL);
    }
    /* print the results */
    printf("The calc_average value is %f\n", AVERAGE);
    printf("The calc_minimum value is %d\n", MINIMUM);
    printf("The calc_maximum value is %d\n", MAXIMUM);

    return 0;
}