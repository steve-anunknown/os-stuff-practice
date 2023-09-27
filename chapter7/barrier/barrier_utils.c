#include "barrier_utils.h"
#include <pthread.h>

static int barrier;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

// initialize barrier
int init(int n)
{
    barrier = n;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    return 0;
}

// wait at barrier
int barrier_point(void)
{
    int ret = 0;
    pthread_mutex_lock(&mutex);
    barrier--;
    if (barrier == 0)
    {
        pthread_cond_broadcast(&cond);
        ret = 1;
    }
    else
    {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    return ret;
}