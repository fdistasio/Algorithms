#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "macros.h"

// fork(), wait()

int main() {
    pid_t pid;

    // Create Child process
    SYSC(pid, fork(), "fork error");

    if (pid == 0) {
       
       // Child process

        printf("I'm the Child process, PID: %d\n", getpid());

        // Exit from child process
        exit(EXIT_SUCCESS);

    } else {

        // Father Process

        printf("I'm the Father process, PID: %d\n", getpid());

        // Wait Child Process
        int status;
        pid_t child_pid;
        SYSC(child_pid, wait(&status), "wait error");


        if (WIFEXITED(status)) {
            printf("Child process exited with code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process exit error \n");
        }

        // Exit from father process
        exit(EXIT_SUCCESS);
        
    }
}
