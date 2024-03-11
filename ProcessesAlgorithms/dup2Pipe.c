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

    // Creazione pipe
    SYSC(ret_value, pipe(pfd), "Errore in Pipe");
    
    // Creazione del processo che eseguira' "ls -l"
    SYSC(pid_ls, fork(), "Errore in fork ls -l");

    if (pid_ls == 0) {
        // Codice del processo figlio

        // Redirezione dello stdout nel file descriptor di scrittura della pipe
        SYSC(ret_value, dup2(pfd[1], fileno(stdout)), "in dup2");

        // Chiusura pipe in lettura e scrittura
        SYSC(ret_value, close(pfd[1]), "in chiusura pipe");
        SYSC(ret_value, close(pfd[0]), "in chiusura pipe");

        // Esecuzione "ls -l"
        SYSC(ret_value, execlp("ls", "ls", "-l", NULL), "in exec");

        // Se la exec fallisce
        perror("in exec");
        exit(EXIT_FAILURE);
    }   

    // Creazione del processo che eseguira' "wc -l"
    SYSC(pid_wc, fork(), "Errore in fork wc -l");

    if (pid_wc == 0) {
        // Codice del processo figlio

        // Redirezione dello stdin nel file descriptor di lettura della pipe
        SYSC(ret_value, dup2(pfd[0], fileno(stdin)), "in dup2");

        // Chiusura pipe in lettura e scrittura
        SYSC(ret_value, close(pfd[0]), "in chiusura pipe");
        SYSC(ret_value, close(pfd[1]), "in chiusura pipe");

        // Esecuzione "wc -l"
        SYSC(ret_value, execlp("wc", "wc", "-l", NULL), "n exec");

        // Se la exec fallisce
        perror("in exec");
        exit(EXIT_FAILURE);
    }

    // Chiusura pipe in lettura e scrittura
    SYSC(ret_value, close(pfd[1]), "in chiusura pipe");    
    SYSC(ret_value, close(pfd[0]), "in chiusura pipe");

    // Attesa della terminazione dei processi figli
    SYSC(ret_value, waitpid(pid_ls, NULL, 0), "in waitpid");
    SYSC(ret_value, waitpid(pid_wc, NULL, 0), "in waitpid");
}
