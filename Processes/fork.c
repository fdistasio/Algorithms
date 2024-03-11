#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "macros.h"

// fork(), wait()

int main() {
    pid_t pid;

    // Creazione di un nuovo processo
    SYSC(pid, fork(), "nella fork");

    if (pid == 0) {
        // Codice eseguito dal processo figlio
        printf("Sono il processo figlio, PID: %d\n", getpid());

        // Terminazione del processo figlio
        exit(EXIT_SUCCESS);

    } else {
        // Codice eseguito dal processo padre
        printf("Sono il processo padre, PID: %d\n", getpid());

        // Attesa della terminazione del processo figlio
        int status;
        pid_t child_pid;
        SYSC(child_pid, wait(&status), "nella wait");

        if (WIFEXITED(status)) {
            printf("Il processo figlio con PID %d che avevo creato come %d è terminato con stato %d\n",
                   child_pid, pid, WEXITSTATUS(status));
        } else {
            printf("Il processo figlio con PID %d non è terminato correttamente\n", child_pid);
        }

        // Terminazione del processo padre
        exit(EXIT_SUCCESS);
    }
}
