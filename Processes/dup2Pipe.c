#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "macros.h"
#include "constants.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// fork(), pipe(), dup2(), execlp()

int main(int argc, char* argv[]){
    
    pid_t pid_ls, pid_wc;
    int pfd[2], ret_value;

    // Create Pipe
    SYSC(ret_value, pipe(pfd), "Pipe error");
    
    // Create Child Process (will exec ls -l)
    SYSC(pid_ls, fork(), "fork error");

    if (pid_ls == 0) {
       
       // Child Process

        // stdout redirection in pipe's write file descriptor
        SYSC(ret_value, dup2(pfd[1], fileno(stdout)), "dup2 error");

        // Close pipe file descriptors (read and write)
        SYSC(ret_value, close(pfd[1]), "close pipe error");
        SYSC(ret_value, close(pfd[0]), "close pipe error");

        // Exec "ls -l"
        execlp("ls", "ls", "-l", NULL);

        // Exec error
        perror("exec");
        exit(EXIT_FAILURE);
    }   

    // Create Child Process (will exec wc -l)
    SYSC(pid_wc, fork(), "fork error");

    if (pid_wc == 0) {
       
       // Child Process

        // stdin redirection in pipe's read file descriptor
        SYSC(ret_value, dup2(pfd[0], fileno(stdin)), "dup2 error");

        // Close pipe file descriptors (read and write)
        SYSC(ret_value, close(pfd[0]), "close pipe error");
        SYSC(ret_value, close(pfd[1]), "close pipe error");

        // Exec "wc -l"
        execlp("wc", "wc", "-l", NULL);

        // Exec error
        perror("exec");
        exit(EXIT_FAILURE);

    }

    // Close pipe file descriptors (read and write)
    SYSC(ret_value, close(pfd[1]), "close pipe error");    
    SYSC(ret_value, close(pfd[0]), "close pipe error");

    // Wait Child processes
    SYSC(ret_value, waitpid(pid_ls, NULL, 0), "waitpid error");
    SYSC(ret_value, waitpid(pid_wc, NULL, 0), "waitpid error");
}
