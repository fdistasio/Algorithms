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

    // Creazione della pipe
    SYSC(retvalue, pipe(pipe_fd), "nella pipe");

    // Creazione di un nuovo processo figlio
    SYSC(pid, fork(), "nella fork");

    if (pid == 0) { 
        // Codice del processo figlio

        // Chiusura pipe in scrittura
        SYSC(retvalue, close(pipe_fd[1]), "nella close");

        // Lettura dalla pipe
        ssize_t n_read, n_written;
        SYSC(n_read, read(pipe_fd[0], buffer, BUFFER_SIZE), "nella read");

        // Scrittura dei dati ricevuti
        SYSC(n_written, write(STDOUT_FILENO, buffer, n_read), "nella write");

        // Chiusura pipe in lettura
        SYSC(retvalue, close(pipe_fd[0]), "nella close");

        exit(EXIT_SUCCESS);

    } else { 
        // Codice del processo padre

        // Chiusura pipe in lettura
        SYSC(retvalue, close(pipe_fd[0]), "nella close");

        // Scrittura dei dati sulla pipe
        ssize_t n_written;
        const char *data_to_send = "Ciao, caro processo figlio!";
        SYSC(n_written, write(pipe_fd[1], data_to_send, strlen(data_to_send)), "nella write");

        // Chiusura pipe in scrittura
        SYSC(retvalue, close(pipe_fd[1]), "nella close");

        pid_t pid_c;
        // Attesa della terminazione del processo figlio
        SYSC(pid_c, waitpid(pid, NULL, 0), "nella waitpid");
    }

    return 0;
}
