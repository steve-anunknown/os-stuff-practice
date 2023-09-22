#include <stdio.h>
#include "stats.h"
#include "task.h"

void update_stats(Stats *stats, Task *task)
{
    stats->avg_waiting_time    += (float)task->waiting_time;
    stats->avg_turnaround_time += (float)task->turnaround_time;
    stats->avg_response_time   += (float)task->response_time;
}
void finalize_stats(Stats *stats, int tids)
{
    stats->avg_waiting_time    /= (float)tids;
    stats->avg_turnaround_time /= (float)tids;
    stats->avg_response_time   /= (float)tids;
}
void print_stats(Stats *stats)
{
    printf("Average waiting time:    %f\n", stats->avg_waiting_time);
    printf("Average turnaround time: %f\n", stats->avg_turnaround_time);
    printf("Average response time:   %f\n", stats->avg_response_time);
}