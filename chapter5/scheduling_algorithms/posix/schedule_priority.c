// implementation of a first come first serve scheduler.
// the task with the earliest arrival time is run first.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "task.h"
#include "stats.h"
#include "list.h"
#include "cpu.h"

// this is the head of the task list.
struct node *head = NULL;
unsigned int timer = 0;
int tids = 0;

void add(char *name, int priority, int burst)
{
    // create a new task
    Task* newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = tids++;
    // suppose that lower numbers mean higher priority
    // search through the list to find the right place to insert the new task
    struct node *temp = head;
    struct node *prev = head;
    while (temp != NULL && priority > temp->task->priority)
    {
        prev = temp;
        temp = temp->next;
    }
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    // special case - beginning of list
    if (temp == head)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        // interior or last element in the list
        prev->next = newNode;
        newNode->next = temp;
    }
}

void update_task_fields(struct node *curr)
{
    // store the burst, otherwise, if the
    // curr task is the head, then the burst
    // will be overwritten by zero before
    // the other tasks can update their waiting times.
    int burst = curr->task->burst;
    struct node *temp = head;
    while (temp != NULL)
    {
        if (temp != curr)
        {
            temp->task->waiting_time += burst;
        }
        else
        {
            Task *curt = temp->task;
            curt->turnaround_time = curt->burst;
            curt->burst = 0;
            curt->response_time = (curt->response_time == 0) ? timer : curt->response_time;
        }
        temp = temp->next;
    }
}

void schedule()
{
    // since the add function already sorts the list,
    // we can just run through the list
    Stats stats = {0.0, 0.0, 0.0};
    struct node *temp = head;
    struct node *prev = head; // for deletion
    while (temp != NULL)
    {
        traverse(head);
        run(temp->task, temp->task->burst);

        timer += temp->task->burst;
        update_task_fields(temp);
        update_stats(&stats, temp->task);

        prev = temp;
        temp = temp->next;
        delete(&head, prev->task);
    }
    finalize_stats(&stats, tids);
    print_stats(&stats);
}