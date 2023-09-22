/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    unsigned int waiting_time;
    unsigned int turnaround_time;
    unsigned int response_time;
} Task;

#endif
