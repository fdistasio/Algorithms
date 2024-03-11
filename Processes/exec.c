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
    SYSC(pid, fork(), "nella fork");

    if (pid == 0) {
        // Codice del processo figlio

        // Utilizzo di exec per eseguire un nuovo programma
        char *args[] = {"ls", "-l", NULL};
        SYSC(retvalue, execvp("ls", args), "nella exec");

        // Se l'esecuzione di exec fallisce
        perror("exec");
        return 1;

    } else {
        // Codice del processo padre

        // Attesa della terminazione del processo figlio
        int status;
        pid_t child_pid;
        SYSC(child_pid, waitpid(pid, &status, 0), "nella waitpid");

        if (WIFEXITED(status)) {
            printf("Il processo figlio è terminato con codice di uscita: %d\n", WEXITSTATUS(status));
        } else {
            printf("Il processo figlio non è terminato correttamente\n");
        }
    }

    return 0;
}