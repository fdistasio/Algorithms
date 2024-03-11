#include <sys/wait.h>
#include <sys/mman.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "macros.h"
#define MEM_SIZE 4096

// fork(), shm_open(), mmap()

int main (int argc, char *ARGV[]) {

    // Controllo degli argomenti
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <shm_name>\n", ARGV[0]);
        exit(EXIT_FAILURE);
    }

    int retvalue, mem_fd;

    // Creazione area di memoria condivisa
    SYSC(mem_fd, shm_open(ARGV[1], O_CREAT | O_RDWR, 0666), "nella shm_open");
    SYSC(retvalue, ftruncate(mem_fd, MEM_SIZE), "nella ftruncate");

    // Creazione di un nuovo processo
    pid_t pid;
    SYSC(pid, fork(), "nella fork");
    
    if (pid == 0) {
            // Codice eseguito dal processo figlio

            // Map della memoria condivisa nello spazio di indirizzamento del processo
            void *ptr;
            SYSCN(ptr, mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0), "nella mmap");

            // Scrittura sulla memoria condivisa
            const char *msg = "Hello, shared memory!\n";
            SYSC(retvalue, sprintf(ptr, "%s", msg), "nella sprintf");

            // Unmap della memoria condivisa
            SYSC(retvalue, munmap(ptr, MEM_SIZE), "nella munmap");

            exit(EXIT_SUCCESS);

    } else {
        // Codice eseguito dal processo padre

        // Map della memoria condivisa nello spazio di indirizzamento del processo
        void *ptr;
        SYSCN(ptr, mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0), "nella mmap");

        // Attesa della terminazione del processo figlio
        SYSC(retvalue, waitpid(pid, NULL, 0), "nella waitpid");
        
        // Scrittura a video il contenuto dell'area di memoria condivisa
        SYSC(retvalue, write(STDOUT_FILENO, ptr, strlen(ptr)), "nella write");

        // Unmap della memoria condivisa
        SYSC(retvalue, munmap(ptr, MEM_SIZE), "nella munmap");
        exit(EXIT_SUCCESS);
    }
}