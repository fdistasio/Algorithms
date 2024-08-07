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

    // Param check
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <shm_name>\n", ARGV[0]);
        exit(EXIT_FAILURE);
    }

    int retvalue, mem_fd;

    // Create shared memory segment
    SYSC(mem_fd, shm_open(ARGV[1], O_CREAT | O_RDWR, 0666), "shm_open error");
    SYSC(retvalue, ftruncate(mem_fd, MEM_SIZE), "ftruncate error");

    // Create Child Process
    pid_t pid;
    SYSC(pid, fork(), "fork error");
    
    if (pid == 0) {

        // Child Process         
                    
        // Shared memory mapping
        void *ptr;
        SYSCN(ptr, mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0), "mmap error");

        // Write on shared memory segment
        const char *msg = "Hello, shared memory!\n";
        SYSC(retvalue, sprintf(ptr, "%s", msg), "sprintf error");
        
        // Shared memory unmapping
        SYSC(retvalue, munmap(ptr, MEM_SIZE), "munmap error");
        exit(EXIT_SUCCESS);

    } else {

        // Father Process

        // Shared memory mapping
        void *ptr;
        SYSCN(ptr, mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0), "mmap error");

        // Wait Child process
        SYSC(retvalue, waitpid(pid, NULL, 0), "waitpid error");
        
        // Write read data on stdout
        SYSC(retvalue, write(STDOUT_FILENO, ptr, strlen(ptr)), "write error");

        // Shared memory unmapping
        SYSC(retvalue, munmap(ptr, MEM_SIZE), "munmap error");
        exit(EXIT_SUCCESS);
    }
}
