#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct range
{
    int start;
    int end;
} range;

int *arr;

void* mergeSort(void* param)
{
    range *r = (range*)param;
    int start = r->start;
    int end = r->end;
    if (start >= end)
    {
        return NULL;
    }
    int mid = (start + end) / 2;
    range r1 = {start, mid};
    range r2 = {mid + 1, end};
    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, mergeSort, &r1);
    pthread_create(&tid2, &attr, mergeSort, &r2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    // merge
    int i = start;
    int j = mid + 1;
    int k = 0;
    int temp[end - start + 1];
    while (i <= mid && j <= end)
    {
        if (arr[i] < arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }
    while (j <= end)
    {
        temp[k++] = arr[j++];
    }
    for (int i = start; i <= end; ++i)
    {
        arr[i] = temp[i - start];
    }
    pthread_exit(0);

}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./mergeSort <n>\n");
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
    pthread_create(&tid, &attr, mergeSort, &r);
    pthread_join(tid, NULL);

    printf("After sorting: ");
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");


    return 0;
}