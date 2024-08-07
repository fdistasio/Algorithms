#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

// fork(), pipe()

int main() {
    int pipe_fd[2], retvalue;
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create Pipe
    SYSC(retvalue, pipe(pipe_fd), "pipe error");

    // Create child process
    SYSC(pid, fork(), "fork error");

    if (pid == 0) { 
        
        // Child Process

        // Close pipe file descriptor (write)
        SYSC(retvalue, close(pipe_fd[1]), "close error");

        // Read from pipe
        ssize_t n_read, n_written;
        SYSC(n_read, read(pipe_fd[0], buffer, BUFFER_SIZE), "read error");

        // Write on pipe
        SYSC(n_written, write(STDOUT_FILENO, buffer, n_read), "write error");

        // Close pipe file descriptor (read)
        SYSC(retvalue, close(pipe_fd[0]), "close error");

        exit(EXIT_SUCCESS);

    } else { 
        
        // Father Process

        // Close pipe file descriptor (read)
        SYSC(retvalue, close(pipe_fd[0]), "close error");

        // Write on pipe
        ssize_t n_written;
        const char *data_to_send = "Hello child process!";
        SYSC(n_written, write(pipe_fd[1], data_to_send, strlen(data_to_send)), "write error");

        // Close pipe file descriptor (write)
        SYSC(retvalue, close(pipe_fd[1]), "nella close");

        pid_t pid_c;

        // Wait child process
        SYSC(pid_c, waitpid(pid, NULL, 0), "nella waitpid");
    }

    return 0;
}
