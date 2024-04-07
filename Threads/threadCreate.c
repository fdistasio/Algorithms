#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "macros.h"

void* thread_fun(void*);

int main(int argc, char *argv[]) {

    pthread_t tid1;
    int argument = 1;
    void * retValue;   

    // Creazione Thread
    SYST(pthread_create(&tid1, NULL, thread_fun, &argument));

    // Allocazione memoria per il valore di ritorno
    SYSCN(retValue, malloc(sizeof(int)), "nella malloc");

    // Attesa terminazione Thread
    SYST(pthread_join(tid1, &retValue));

    if(retValue != NULL) {
        printf("Valore restituito dal thread: %d\n", *(int*)retValue);
        free(retValue);
    } else {
        printf("Errore terminazione Thread");
        exit(EXIT_FAILURE);
    }
}

void* thread_fun(void *arg) {
    
    int *retData;
    pthread_t tid;

    // Tid del thread
    SYST((tid = pthread_self()));

    printf("Thread %d, value: %d\n", tid, *(int*)arg);

    // Allocazione memoria per il valore di ritorno
    SYSCN(retData, malloc(sizeof(int)), "nella malloc");

    *retData = 69;

    // Terminazione del thread
    pthread_exit(retData);
}
