/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// next index
int next;

// atomic variable for operation
int op;

// operation state
pthread_cond_t available;

// mutex
pthread_mutex_t oper_mutex;
pthread_mutex_t task_mutex;

// the work queue
task worktodo[QUEUE_SIZE];

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    pthread_mutex_lock(&task_mutex);
    if (next == QUEUE_SIZE)
    {
        pthread_mutex_unlock(&task_mutex);
        return 1;
    }
    worktodo[next] = t;
    ++next; // atomic
    pthread_mutex_unlock(&task_mutex);
    pthread_cond_signal(&available);
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    pthread_mutex_lock(&task_mutex);
    if (next <= 0)
    {
        pthread_mutex_unlock(&task_mutex);
        task ret = {NULL,NULL};
        return ret;
    }
    --next; // atomic
    pthread_mutex_unlock(&task_mutex);
    return worktodo[next];
}

// the worker thread in the thread pool
void *worker(void *param)
{
    pthread_mutex_lock(&oper_mutex);
    while (op)
    {
        pthread_mutex_unlock(&oper_mutex);

        pthread_mutex_lock(&task_mutex);
        while (next <= 0)
        {
            pthread_cond_wait(&available,&task_mutex);
        }
        pthread_mutex_unlock(&task_mutex);

        task work = dequeue();
        if (work.function == NULL)
        {
            continue;
        }
        execute(work.function,work.data);

        pthread_mutex_lock(&oper_mutex);
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task currtask = {somefunction,p};
    int ret = enqueue(currtask);
    return ret;
}

// initialize the thread pool
int pool_init(void)
{
    int ret = 0;
    next = 0;
    op = 1;
    pthread_mutex_init(&oper_mutex,NULL);
    pthread_mutex_init(&task_mutex,NULL);
    pthread_cond_init(&available,NULL);
    
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        ret = pthread_create(&bee[i],NULL,worker,NULL);
        if (ret)
        {
            return ret;
        }
    }
    return 0;
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_mutex_lock(&oper_mutex);
    op = 0;
    pthread_mutex_unlock(&oper_mutex);

    pthread_mutex_lock(&task_mutex);
    next = QUEUE_SIZE;
    pthread_mutex_unlock(&task_mutex);

    pthread_cond_broadcast(&available);
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(bee[i],NULL);
    }
}
