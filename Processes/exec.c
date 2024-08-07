#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include "macros.h"

// fork(), exec(), waitpid()

int main() {

    pid_t pid;
    int retvalue;
    SYSC(pid, fork(), "fork error");

    if (pid == 0) {
        // Child Process

        // Exec ls -l
        char *args[] = {"ls", "-l", NULL};
        execvp("ls", args);

        // Exec error
        perror("exec");
        return 1;

    } else {

        // Father Process

        // Wait Child Process
        int status;
        pid_t child_pid;
        SYSC(child_pid, waitpid(pid, &status, 0), "waitpid error");

        if (WIFEXITED(status)) {
            printf("Child process exited with code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process exit error \n");
        }
    }

    return 0;
}