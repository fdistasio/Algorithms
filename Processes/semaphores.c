#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <errno.h>
#include "macros.h"

#define BUFFER_SIZE 10
#define SHARED_MEM_NAME "/shared_memory"
#define SEM_MUTEX_NAME "/sem_mutex"
#define SEM_EMPTY_NAME "/sem_empty"
#define SEM_FULL_NAME "/sem_full"

typedef struct {
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index;
} SharedData;

void writer(sem_t*, sem_t*, sem_t*, SharedData*);
void reader(sem_t*, sem_t*, sem_t*, SharedData*);


int main() {
    int shm_fd,retvalue;
    sem_t *sem_mutex, *sem_empty, *sem_full;
    SharedData *shared_data;

    // Creazione e apertura della memoria condivisa
    SYSC(shm_fd, shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666), "shm_open");

    // Impostazione della dimensione della memoria condivisa
    SYSC(retvalue, ftruncate(shm_fd, sizeof(SharedData)), "nella ftruncate");

    // Mapping della memoria condivisa
    SYSCN(shared_data, (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0), "mmap");

    // Creazione e inizializzazione dei semafori
    SYSCN(sem_mutex, sem_open(SEM_MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1), "sem_mutex: open");
    SYSCN(sem_empty, sem_open(SEM_EMPTY_NAME, O_CREAT | O_EXCL, 0666, 0), "sem_empty: open");
    SYSCN(sem_full, sem_open(SEM_FULL_NAME, O_CREAT | O_EXCL, 0666, BUFFER_SIZE), "sem_full: open");


    // Inizializzazione del buffer circolare
    shared_data->read_index = 0;
    shared_data->write_index = 0;

    // Creazione di processi lettori e scrittori (da implementare)
    // Creazione di processi lettori e scrittori
    SYSC(retvalue, fork(), "nella fork");
    if (!retvalue) {
        // Processo figlio (lettore)
        reader(sem_mutex, sem_empty, sem_full, shared_data);
    } else {
        // Processo padre (scrittore)
        SYSC(retvalue, fork(), "nella fork");
        if (!retvalue) {
            writer(sem_mutex, sem_empty, sem_full, shared_data);
        }
    }

    // Attesa dei processi figli
    SYSC(retvalue, wait(NULL), "nella wait");

    // Chiusura delle risorse
    SYSC(retvalue, munmap(shared_data, sizeof(SharedData)), "nella munmap");
    SYSC(retvalue, close(shm_fd), "nella close");

    SYSC(retvalue, sem_close(sem_mutex), "nella sem_close");
    SYSC(retvalue, sem_close(sem_empty), "nella sem_close");
    SYSC(retvalue, sem_close(sem_full), "nella sem_close");

    SYSC(retvalue, sem_unlink(SEM_MUTEX_NAME), "nella sem_unlink");
    SYSC(retvalue, sem_unlink(SEM_EMPTY_NAME), "nella sem_unlink");
    SYSC(retvalue, sem_unlink(SEM_FULL_NAME), "nella sem_unlink");

    SYSC(retvalue, shm_unlink(SHARED_MEM_NAME), "nella shm_unlink");

    return 0;
}

void reader(sem_t *sem_mutex, sem_t *sem_empty, sem_t *sem_full, SharedData *shared_data) {
    char data;
    int retvalue;

    while (1) {
        // Attendi che ci siano dati disponibili nel buffer
        SYSC(retvalue, sem_wait(sem_empty), "nella sem_wait");
        // Proteggi l'accesso al buffer
        SYSC(retvalue, sem_wait(sem_mutex), "nella sem_wait");

        // Leggi dal buffer
        data = shared_data->buffer[shared_data->read_index];
        shared_data->read_index = (shared_data->read_index + 1) % BUFFER_SIZE;

        // Rilascia la protezione del buffer
        SYSC(retvalue, sem_post(sem_mutex), "nella sem_post");
        // Incrementa il numero di spazi vuoti nel buffer
        SYSC(retvalue, sem_post(sem_full), "nella sem_post");

        // Utilizza il dato letto (in questo caso, stampa a schermo)
        printf("Reader - Read: %c\n", data);

        if(data == 'Z') exit(EXIT_SUCCESS);

        // Simula un tempo di elaborazione
        usleep(100000);
    }
}

void writer(sem_t *sem_mutex, sem_t *sem_empty, sem_t *sem_full, SharedData *shared_data) {
    char data = 'A';
    int retvalue;

    while (1) {
        // Attendi che ci siano spazi vuoti nel buffer
        SYSC(retvalue, sem_wait(sem_full), "nella sem_wait");
        // Proteggi l'accesso al buffer
        SYSC(retvalue, sem_wait(sem_mutex), "nella sem_wait");

        // Scrivi nel buffer
        shared_data->buffer[shared_data->write_index] = data;
        shared_data->write_index = (shared_data->write_index + 1) % BUFFER_SIZE;

        // Rilascia la protezione del buffer
        SYSC(retvalue, sem_post(sem_mutex), "nella sem_post");
        // Incrementa il numero di dati disponibili nel buffer
        SYSC(retvalue, sem_post(sem_empty), "nella sem_post");

        // Utilizza il dato scritto (in questo caso, stampa a schermo)
        printf("Writer - Wrote: %c\n", data);

        // Incrementa il carattere da scrivere (esempio: A, B, C, ...)
        //data = (data + 1) > 'Z' ? 'A' : (data + 1);
        if(data == 'Z') exit(EXIT_SUCCESS);
        data = data + 1;

        // Simula un tempo di elaborazione
        usleep(150000);
    }
}
