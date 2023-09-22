// implementation of a round robin scheduler.
// there is a time quantum for each task in the list.
// once the quantum runs out, the task is moved to the end of the list.
// the next task is then run.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "task.h"
#include "list.h"
#include "stats.h"
#include "cpu.h"

// this is the head of the task list.
struct node *head = NULL;
unsigned int tids = 0;
unsigned int timer = 0;

// this adds a task to the list.
void add(char *name, int priority, int burst)
{
    // create a new task
    Task* newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = tids++;
    newTask->waiting_time = 0;
    newTask->turnaround_time = 0;
    newTask->response_time = 0;
    insert(&head, newTask);
}

void update_task_fields(struct node *curr)
{
    struct node *temp = head;
    while (temp != NULL)
    {
        if (temp != curr)
        {
            temp->task->waiting_time += QUANTUM;
        }
        else
        {
            Task *curt = temp->task;
            curt->turnaround_time += QUANTUM;
            curt->burst -= QUANTUM;
            curt->response_time = (curt->response_time == 0) ? timer : curt->response_time;
        }
        temp = temp->next;
    }
}

void schedule()
{
    printf("Running round robin scheduler...\n");
    printf("MAX TID: %d\n", tids);

    Stats stats = {0.0, 0.0, 0.0};

    struct node *curr = head;
    struct node *prev = head; // for deletion
    while (curr != NULL)
    {
        traverse(head);
        run(curr->task, QUANTUM);
        timer += QUANTUM;
        update_task_fields(curr);
        if (curr->task->burst <= 0)
        {
            update_stats(&stats, curr->task);
            // delete the task
            struct node* temp = curr;
            temp = curr->next;
            delete(&head, curr->task);
            curr = (temp == NULL) ? head : temp;
        }
        else
        {
            prev = curr;
            curr = (curr->next == NULL) ? head : curr->next;
        }
    }
    finalize_stats(&stats, tids);
    print_stats(&stats);
}