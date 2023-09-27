#include <stdio.h>
#include <pthread.h>
#include "pidAPI.h"

// use mutex to synchronize access to pidMap
pthread_mutex_t mutex;

// bitmap: 0 means available, 1 means allocated
int pidMap[MAX_PID - MIN_PID + 1];

int allocate_map(void)
{
    // initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Error initializing mutex\n");
        return -1;
    }
    int i;
    for (i = 0; i < MAX_PID - MIN_PID + 1; i++)
    {
        pidMap[i] = 0;
    }
    return 1;
}

int allocate_pid(void)
{
    int i;
    // search for first available pid
    for (i = 0; i < MAX_PID - MIN_PID + 1; i++)
    {
        // lock mutex before accessing pidMap
        pthread_mutex_lock(&mutex);
        if (pidMap[i] == 0)
        {
            pidMap[i] = 1;
            // unlock mutex after accessing pidMap
            pthread_mutex_unlock(&mutex);
            return i + MIN_PID;
        }
        pthread_mutex_unlock(&mutex);
    }
    return -1; // no pid available
}

void release_pid(int pid)
{
    if (pid < MIN_PID || pid > MAX_PID)
    {
        fprintf(stderr, "Invalid pid\n");
        return;
    }
    // this does not need to be synchronized because
    // it is only called by the thread that owns the pid
    pidMap[pid - MIN_PID] = 0;
}