#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct range
{
    int start;
    int end;
} range;

int *arr;

void* quickSort(void* param)
{
    range* r = (range*)param;
    int start = r->start;
    int end = r->end;
    if (start >= end)
    {
        return NULL;
    }
    int pivot = start;
    int i = start + 1;
    int j = end;
    while (i <= j)
    {
        while (i <= end && arr[i] <= arr[pivot])
        {
            ++i;
        }
        while (j > start && arr[j] >= arr[pivot])
        {
            --j;
        }
        if (i > j)
        {
            int temp = arr[j];
            arr[j] = arr[pivot];
            arr[pivot] = temp;
        }
        else
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    range r1 = {start, j - 1};
    range r2 = {j + 1, end};
    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, quickSort, &r1);
    pthread_create(&tid2, &attr, quickSort, &r2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./quickSort <n>\n");
        exit(-1);
    }
    if (atoi(argv[1]) <= 0)
    {
        fprintf(stderr, "Error: Invalid n value\n");
        exit(-1);
    }
    int n = atoi(argv[1]);
    arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i)
    {
        arr[i] = rand() % 100;
    }
    printf("Before sorting: ");
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    range r = {0, n - 1};
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, quickSort, &r);
    pthread_join(tid, NULL);

    printf("After sorting: ");
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");


    return 0;
}