/*

    A program that mimics the cp command in Linux
    using the POSIX API.

*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <source> <target>\n", argv[0]);
        return -1;
    }

    const char* source = argv[1];
    const char* target = argv[2];
    if (strcmp(source, target) == 0)
    {
        fprintf(stderr, "Source and target must be different\n");
        return -1;
    }
    
    int source_fd, target_fd;
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
    {
        perror("open: Error opening source file");
        return -1;
    }
    target_fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (target_fd == -1)
    {
        perror("open: Error opening target file. File already exists");
        close(source_fd);
        return -1;
    }

    const int bytes = 1024;
    char buffer[bytes];
    ssize_t bytes_read;
    ssize_t bytes_written;
    
    while((bytes_read = read(source_fd, buffer, bytes)) > 0)
    {
        // attempt to write whatever was read
        bytes_written = write(target_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            perror("write: Error writing to target file");
            close(source_fd);
            close(target_fd);
            return -1;
        }
    }
    if (bytes_read == -1)
    {
        perror("read: Error reading from source file");
        close(source_fd);
        close(target_fd);
        return -1;
    }

    close(source_fd);
    close(target_fd);
    return 0;
}