#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 100

int mutexProtectedGlobalVariable;
int unprotectedGlobalVariable;

pthread_mutex_t myMutex;

void* unprotectedThreadFunc(void* argument) {
    // this function will update the value without any protection
    for (int i = 0; i < 10000; ++i) {
        unprotectedGlobalVariable++;
    }
}

void* protectedThreadFunc(void* argument) {
    // this function will update value with mutex lock protection

    // will *block* thread if myMutex object is locked
    pthread_mutex_lock(&myMutex);

    for (int i = 0; i < 10000; ++i) {
        mutexProtectedGlobalVariable++;
    }

    pthread_mutex_unlock(&myMutex);
}

int main() {
    mutexProtectedGlobalVariable = 0;
    unprotectedGlobalVariable = 0;

    // create array of threads
    pthread_t myThreads[NUM_THREADS];


    printf("Calling unprotected set of threads\n");
    /* 5 threads will call a function that will update the variable unprotected */

    for (int i = 0; i < NUM_THREADS; ++i) {
        int createStatus = pthread_create(&myThreads[i], NULL, unprotectedThreadFunc, (void*)(long)i);
        if (createStatus != 0) {
            fprintf(stderr, "Error creating unprotected thread %d\n", i);
        }
    }


    for (int i = 0; i < NUM_THREADS; ++i) {
        int joinStatus = pthread_join(myThreads[i], NULL);
        if (joinStatus != 0) {
            fprintf(stderr, "Error joining unprotected thread %d\n", i);
        }
    }

    /////////////////////////////////////////////////////////////////////

    /* 5 threads will call a function that will update the variable protected */
    for (int i = 0; i < NUM_THREADS; ++i) {
        int createStatus = pthread_create(&myThreads[i], NULL, protectedThreadFunc, (void*)(long)i);
        if (createStatus != 0) {
            fprintf(stderr, "Error creating protected thread %d\n", i);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        int joinStatus = pthread_join(myThreads[i], NULL);
        if (joinStatus != 0) {
            fprintf(stderr, "Error joining protected thread %d\n", i);
        }
    }


    // Destroy the Mutex object
    pthread_mutex_destroy(&myMutex);

    printf("Protected sum is %d\n", mutexProtectedGlobalVariable);
    printf("Unprotected Sum is %d\n", unprotectedGlobalVariable);

    return 0;
}