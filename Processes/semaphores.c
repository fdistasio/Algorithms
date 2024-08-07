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

    // Create shared memory segment
    SYSC(shm_fd, shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666), "shm_open error");
    SYSC(retvalue, ftruncate(shm_fd, sizeof(SharedData)), "ftruncate error");

    // Shared memory mapping
    SYSCN(shared_data, (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0), "mmap error");

    // Create semaphores
    SYSCN(sem_mutex, sem_open(SEM_MUTEX_NAME, O_CREAT | O_EXCL, 0666, 1), "sem_mutex: open");
    SYSCN(sem_empty, sem_open(SEM_EMPTY_NAME, O_CREAT | O_EXCL, 0666, 0), "sem_empty: open");
    SYSCN(sem_full, sem_open(SEM_FULL_NAME, O_CREAT | O_EXCL, 0666, BUFFER_SIZE), "sem_full: open");

    // Circular buffer init
    shared_data->read_index = 0;
    shared_data->write_index = 0;

    // Create Child process
    SYSC(retvalue, fork(), "fork error");

    if (!retvalue) {

        // Child Process (reader)
        reader(sem_mutex, sem_empty, sem_full, shared_data);

    } else {
        
        // Father Process

        // Create Child process
        SYSC(retvalue, fork(), "fork error");
        
        if (!retvalue) {

            // Child Process (writer)
            writer(sem_mutex, sem_empty, sem_full, shared_data);
        }
    }

    // Wait Child processes
    SYSC(retvalue, wait(NULL), "wait error");

    // Close resources
    SYSC(retvalue, munmap(shared_data, sizeof(SharedData)), "munmap error");
    SYSC(retvalue, close(shm_fd), "close error");

    SYSC(retvalue, sem_close(sem_mutex), "sem_close error");
    SYSC(retvalue, sem_close(sem_empty), "sem_close error");
    SYSC(retvalue, sem_close(sem_full), "sem_close error");

    SYSC(retvalue, sem_unlink(SEM_MUTEX_NAME), "sem_unlink error");
    SYSC(retvalue, sem_unlink(SEM_EMPTY_NAME), "sem_unlink error");
    SYSC(retvalue, sem_unlink(SEM_FULL_NAME), "sem_unlink error");

    SYSC(retvalue, shm_unlink(SHARED_MEM_NAME), "shm_unlink error");

    return 0;
}

void reader(sem_t *sem_mutex, sem_t *sem_empty, sem_t *sem_full, SharedData *shared_data) {

    char data;
    int retvalue;

    while (1) {
        
        // Wait data
        SYSC(retvalue, sem_wait(sem_empty), "sem_wait error");
       
        // Mutex lock
        SYSC(retvalue, sem_wait(sem_mutex), "sem_wait error");

        // Read from buffer
        data = shared_data->buffer[shared_data->read_index];
        shared_data->read_index = (shared_data->read_index + 1) % BUFFER_SIZE;

        // Mutex unlock
        SYSC(retvalue, sem_post(sem_mutex), "sem_post error");

        // Increment data on buffer full counter
        SYSC(retvalue, sem_post(sem_full), "sem_post error");

        // Write read data on stdout
        printf("Reader - Read: %c\n", data);

        if(data == 'Z') exit(EXIT_SUCCESS);

        // Simulate elaboration time
        usleep(100000);
    }

}

void writer(sem_t *sem_mutex, sem_t *sem_empty, sem_t *sem_full, SharedData *shared_data) {

    char data = 'A';
    int retvalue;

    while (1) {
        
        // Wait free slots
        SYSC(retvalue, sem_wait(sem_full), "sem_wait error");
        
        // Mutex lock
        SYSC(retvalue, sem_wait(sem_mutex), "sem_wait error");

        // Write on buffer
        shared_data->buffer[shared_data->write_index] = data;
        shared_data->write_index = (shared_data->write_index + 1) % BUFFER_SIZE;

        // Mutex unlock
        SYSC(retvalue, sem_post(sem_mutex), "sem_post error");
        
        // Increment data on buffer empty counter
        SYSC(retvalue, sem_post(sem_empty), "sem_post error");

        // Write wrote data on stdout
        printf("Writer - Wrote: %c\n", data);

        // Change char
        if(data == 'Z') exit(EXIT_SUCCESS);
        data = data + 1;

        // Simulate elaboration time
        usleep(150000);
    }
}
