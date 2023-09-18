/*
    This program implements a simple shell in C, called minishell.
    It is a project from the book
        Operating Systems Concepts, 10th Edition
        Abraham Silberschatz, Peter B. Galvin, Greg Gagne
    It has very limited funtionality.
    It can only handle one pipe, one input redirection and one output redirection.
    It cannot chain commands with pipes, input redirections or output redirections.
    It doesn't properly handle quotes. For example the command:
        > echo "hello\nworld" will output "hello\nworld" instead of "hello" and "world" on separate lines.
    Basically, one thing at a time.
    It is also pretty badly written but I may fix it in the future.
    It ain't much, but it's honest work.
*/
#include "minishell_utils.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */


int main(void)
{
    char* args[MAX_LINE/2 + 1]; /* command line arguments */
    char* nextargs[MAX_LINE/2 + 1];
    char lastCommand[MAX_LINE];
    char lastCommandCopy[MAX_LINE];
    char line[MAX_LINE];
    char lineCopy[MAX_LINE];
    char file[MAX_LINE];
    int pipeIndex;

    int should_run = 1; /* flag to determine when to exit program */
    bool history = false;
    while(should_run)
    {
        int commandIndex    = 0;
        bool backgroundFlag = false;
        bool pipeFlag       = false;
        bool inputRedirFlag = false;
        bool outputRedirFlag= false;

        printf("minishell>");
        fflush(stdout);
        if (!history)
        {
            fgets(line, MAX_LINE, stdin);
            strcpy(lineCopy, line);
            splitLineToWords(lineCopy, args);
        }
        else
        {
            printf("%s", lastCommand);
            history = false;
            strcpy(lastCommandCopy, lastCommand);
            splitLineToWords(lastCommandCopy, args);
        }

        bool trigger = false;
        while (args[commandIndex] != NULL)
        {
            trim(args[commandIndex]);
            if ((inputRedirFlag  |= !strcmp(args[commandIndex], "<")) ||
                (outputRedirFlag |= !strcmp(args[commandIndex], ">")))
            {
                args[commandIndex++] = NULL;
                strcpy(file, args[commandIndex]);
                trim(file);
                break;
            }
            if ((backgroundFlag = !strcmp(args[commandIndex], "&")))
            {
                args[commandIndex] = NULL;
                break;
            }
            if ((pipeFlag |= !strcmp(args[commandIndex], "|")) && !trigger)
            {
                trigger = true;
                args[commandIndex] = NULL;
                pipeIndex = commandIndex;
            }
            commandIndex++;
        }
        memcpy(nextargs, args + pipeIndex + 1, sizeof(args));

        if (checkHistory(args, &history, lastCommand, line)) continue;
        checkExit(args);
        executeCommand(args, nextargs, file,
                       backgroundFlag, inputRedirFlag, outputRedirFlag, pipeFlag);
    }

    return 0;
}