#define MIN_PID 300
#define MAX_PID 5000

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
    return 1;
}

void release_pid(int pid)
{
    pidMap[pid - MIN_PID] = 0;
}