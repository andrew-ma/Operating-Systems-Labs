#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ARRSIZE 5

// thread local variable, meaning while it is still global, the modified value will be thread specific
__thread int myVal;
__thread int myArr[ARRSIZE];

void printMyVal(long threadID) {
    /* Prints thread local values */
    printf("Thread local 'myVal' for thread %ld is %d\n", threadID, myVal);

    for (int i = 0; i < ARRSIZE; ++i) {
        printf("Thread local 'myArr[%d]' for thread %ld is %d\n", i, threadID, myArr[i]);
    }
}

void* simpleThreadFunc(void* argument) {
    long threadID = (long)argument;
    printf("Thread ID is %ld\n", threadID);

    // set thread local value
    myVal = threadID * 100;

    // set thread local array values
    for (int i = 0; i < 5; ++i) {
        myArr[i] = (threadID * 100 + i);
    }

    printMyVal(threadID);
}


int main() {
    // create array of Threads
    pthread_t myThreads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Creating thread %d, and sending ID %d\n", i, i);
        int createStatus = pthread_create(&myThreads[i], NULL, simpleThreadFunc, (void*)(long)(i));
        if (createStatus) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return -1;
        }
        else {
            printf("Successfully created thread %d\n", i);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        int joinStatus = pthread_join(myThreads[i], NULL);
        if (joinStatus != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return -1;
        } else {
            printf("Successful termination of thread %d\n", i);
        }
    }

    return 0;
}