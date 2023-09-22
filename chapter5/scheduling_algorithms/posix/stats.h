#ifndef STATS_H
#define STATS_H

#include "task.h"
typedef struct stats
{
    float avg_waiting_time;
    float avg_turnaround_time;
    float avg_response_time;
} Stats;

void update_stats(Stats *stats, Task *task);
void finalize_stats(Stats *stats, int num_tasks);
void print_stats(Stats *stats);

#endif