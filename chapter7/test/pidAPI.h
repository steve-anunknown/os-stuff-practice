#ifndef PIDAPI_H
#define PIDAPI_H

#define MIN_PID 300
#define MAX_PID 5000

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

#endif
