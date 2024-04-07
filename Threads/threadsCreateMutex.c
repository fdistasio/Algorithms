#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "macros.h"

#define NUM_THREADS 5

// Dichiarazione mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_fun(void*);

int main(int argc, char *argv[]) {

    pthread_t tid[NUM_THREADS];
    int arguments[NUM_THREADS];

    // Creazione dei Threads
    for(int i = 0 ; i < NUM_THREADS ; i++) {
        arguments[i] = i;
        SYST(pthread_create(&tid[i], NULL, thread_fun, &arguments[i]));
    }
   
    // Attesa terminazione dei Threads
    for (int i = 0 ; i < NUM_THREADS ; i++) {
        SYST(pthread_join(tid[i], NULL));
    }
}

void* thread_fun(void *arg) {
    
    pthread_mutex_lock(&mutex);

    static int sum = 0;
    sum += *(int*)arg;
    printf("Thread %d: valore %d\n", *(int*)arg, sum);

    pthread_mutex_unlock(&mutex);
    return NULL;
}