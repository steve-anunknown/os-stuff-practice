#include <stdio.h>
#include "pidAPI.h"

// bitmap: 0 means available, 1 means allocated
int pidMap[MAX_PID - MIN_PID + 1];

int allocate_map(void)
{
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
    for (i = 0; i < MAX_PID - MIN_PID + 1; i++)
    {
        if (pidMap[i] == 0)
        {
            pidMap[i] = 1;
            return i + MIN_PID;
        }
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
    pidMap[pid - MIN_PID] = 0;
}