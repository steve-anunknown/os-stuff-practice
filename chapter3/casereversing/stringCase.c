#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define READ_END 0
#define WRITE_END 1

void reverseCase(const char* original, char* reversed)
{
    int i = 0;
    while (original[i] != '\0')
    {
        if (original[i] >= 'a' && original[i] <= 'z')
        {
            reversed[i] = original[i] - 32;
        }
        else if (original[i] >= 'A' && original[i] <= 'Z')
        {
            reversed[i] = original[i] + 32;
        }
        else
        {
            reversed[i] = original[i];
        }
        i++;
    }
    reversed[i] = '\0';
}

int main(void)
{

    int fd_original[2];
    int fd_modified[2];
    if ((pipe(fd_original) == -1) || (pipe(fd_modified) == -1))
    {
        perror("pipe");
        exit(-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork failed\n");
    }
    else if (pid == 0)
    {
        // child process
        char original[64];
        char reversed[64];

        close(fd_original[WRITE_END]);
        close(fd_modified[READ_END]);
        read(fd_original[READ_END], original, sizeof(original));
        close(fd_original[READ_END]);
        reverseCase(original, reversed);
        write(fd_modified[WRITE_END], reversed, strlen(reversed) + 1);
        close(fd_modified[WRITE_END]);
    }
    else
    {
        // parent process
        char original[64];
        char reversed[64];
        printf("Enter a string: ");
        fgets(original, sizeof(original), stdin);

        close(fd_modified[WRITE_END]);
        close(fd_original[READ_END]);
        write(fd_original[WRITE_END], original, strlen(original) + 1);
        close(fd_original[WRITE_END]);
        read(fd_modified[READ_END], reversed, sizeof(reversed));
        close(fd_modified[READ_END]);
        printf("Reversed string: %s\n", reversed);
    }
    return 0;
}