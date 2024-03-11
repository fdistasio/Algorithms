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

    // Creazione della pipe
    SYSC(retvalue, pipe(pipe_fd), "nella creazione della pipe");

    // Creazione di un processo figlio
    pid_t pid;
    SYSC(pid, fork(), "nella fork");

    if (pid == 0) {
        // Codice del processo figlio

        // Chiusura pipe in lettura
        SYSC(retvalue, close(pipe_fd[0]), "nella close");

        // Redirezione dello stdout nel file descriptor di scrittura della pipe
        SYSC(retvalue, dup2(pipe_fd[1], STDOUT_FILENO), "nella dup2");

        // Chiusura del file descriptor duplicato
        SYSC(retvalue, close(pipe_fd[1]), "nella close");

        // Esecuzione di "ls -la"
        SYSC(retvalue, execlp("ls", "ls -la", (char *) NULL), "nella exec");

        // Se execlp fallisce
        perror("execlp");
        exit(EXIT_FAILURE);

    } else {
        // Codice del processo padre

        char buffer[BUFFER_SIZE];

        // Chiusura pipe in scrittura
        SYSC(retvalue, close(pipe_fd[1]), "nella close");

        // Lettura dei dati dalla pipe
        ssize_t n_read;
        SYSC(n_read, read(pipe_fd[0], buffer, BUFFER_SIZE), "nella read");

        // Scrittura sullo stdout dei dati ricevuti
        SYSC(retvalue, write(STDOUT_FILENO, buffer, n_read), "nella write");

        // Chiusura pipe in lettura
        SYSC(retvalue, close(pipe_fd[0]), "nella close");

        // Attesa della terminazione del processo figlio
        SYSC(retvalue, waitpid(pid, NULL, 0), "nella waitpid");
    }

    return 0;
}