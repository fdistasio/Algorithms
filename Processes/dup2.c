#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "macros.h"

#define BUFFER_SIZE 1024

// fork(), pipe(), dup2(), execlp()

int main() {
    int retvalue, pipe_fd[2];

    // Create pipe
    SYSC(retvalue, pipe(pipe_fd), "pipe error");

    // Create Child Process

    pid_t pid;
    SYSC(pid, fork(), "fork error");

    if (pid == 0) {

        // Child Process

        // Close pipe file descriptor (read)
        SYSC(retvalue, close(pipe_fd[0]), "close error");

        // stdout redirection in pipe's write file descriptor
        SYSC(retvalue, dup2(pipe_fd[1], STDOUT_FILENO), "dup2 error");

        // Close pipe file descriptor (write)
        SYSC(retvalue, close(pipe_fd[1]), "close error");

        // Exec "ls -la"
        execlp("ls", "ls -la", (char *) NULL);

        // Exec error
        perror("execlp");
        exit(EXIT_FAILURE);

    } else {

        // Father Process

        char buffer[BUFFER_SIZE];

        // Close pipe file descriptor (write)
        SYSC(retvalue, close(pipe_fd[1]), "close error");

        // Read from pipe
        ssize_t n_read;
        SYSC(n_read, read(pipe_fd[0], buffer, BUFFER_SIZE), "read error");

        // Write on pipe
        SYSC(retvalue, write(STDOUT_FILENO, buffer, n_read), "write error");

        // Close pipe file descriptor (read)
        SYSC(retvalue, close(pipe_fd[0]), "close error");

        // Wait child process
        SYSC(retvalue, waitpid(pid, NULL, 0), "waitpid error");
    }

    return 0;
}