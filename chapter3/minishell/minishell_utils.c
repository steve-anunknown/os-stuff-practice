
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "minishell_utils.h"

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}
char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}
char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

void splitLineToWords(char *line, char* words[])
{
    int i = 0;
    char *word = strtok(line, " ");
    while (word != NULL)
    {
        words[i++] = word;
        word = strtok(NULL, " ");
    }
    words[i] = NULL;
}

void executeCommand(char* args[], char* nextargs[], char* file, bool background, bool inputRedir, bool outputRedir, bool pipef)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // child process
        if (inputRedir || outputRedir)
        {
            int fd = (inputRedir) ? open(file, O_RDONLY) : open(file, O_WRONLY | O_CREAT, 0666);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }
            if (dup2(fd, (inputRedir) ? STDIN_FILENO : STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            } // the file will close when the child process exits
        }
        else if (pipef)
        {
            int fd[2];
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(1);
            }
            pid_t pid2 = fork();
            if (pid2 < 0)
            {
                perror("fork");
                exit(1);
            }
            else if (pid2 == 0)
            {
                // child process
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(1);
                }
                close(fd[0]);
                close(fd[1]);
                execvp(args[0], args);
                perror("execvp");
                exit(1);
            }
            else
            {
                // parent process
                if (dup2(fd[0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(1);
                }
                close(fd[0]);
                close(fd[1]);
                execvp(nextargs[0], nextargs);
                perror("execvp");
                exit(1);
            }
        }
        /* FOR THE TIME BEING, THIS WORKS ONLY IF THE USER ENTERS A SINGLE COMMAND */
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }
    else
    {
        // parent process
        if (!background)
        {
            wait(NULL);
        }
    }   
}

void checkExit(char* args[])
{
    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
}
bool checkHistory(char* args[], bool* flag, char* last, char* curr)
{
    if (strcmp(args[0], "!!") == 0)
    {
        *flag = true;
        if (strlen(last) == 0)
        {
            printf("No commands in history.\n");
        }
        else
        {
            strcpy(curr, last);
        }   
        return true;
    }
    else
    {
        *flag = false;
        strcpy(last, curr);
    }
    return false;
}
