#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./filecopy <source> <destination>\n");
        return 1;
    }
    if (strcmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "Source and destination must be different\n");
        return 1;
    }

    char buffer[16];
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(-1);
    }
    else if (pid == 0)
    {
        // child process
        close(fd[WRITE_END]);

        int fd_target = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
        if (fd_target == -1) // this fails, among other reasons, if the file already exists
        {
            perror("open: target");
            return 1;
        }
        int bytes_read = 0;
        do
        {
            // read from pipe
            bytes_read = read(fd[READ_END], buffer, sizeof(buffer));
            // write to target file
            write(fd_target, buffer, bytes_read);
        } while (bytes_read > 0);
        close(fd[READ_END]);
        close(fd_target);
    }
    else
    {
        // parent process
        close(fd[READ_END]);

        int fd_source = open(argv[1], O_RDONLY);
        if (fd_source == -1)
        {
            perror("open: source");
            return 1;
        }
        int bytes_to_write = 0;
        do
        {
            // read from source file
            bytes_to_write = read(fd_source, buffer, sizeof(buffer));
            // write to pipe
            write(fd[WRITE_END], buffer, bytes_to_write);
        } while (bytes_to_write > 0);
        close(fd[WRITE_END]);
        close(fd_source);
    }
    return 0;
}