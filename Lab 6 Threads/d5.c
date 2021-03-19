#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ARRSIZE 5

// we will use a key defined by pthread_key_t to define a key
pthread_key_t valKey;

void printMyVal(long threadID) {
    printf("Getting specific value for thread %ld using key\n", threadID);

    int* myVal = pthread_getspecific(valKey);

    printf("The thread local value in thread %ld is %d\n", threadID, *myVal);
}

void* simpleThreadFunc(void* argument) {
    long threadID = (long)argument;

    int myVal = (int)(threadID * 100);

    printf("Creating the variable %d that will be referred to by pthread_key from thread %ld\n", myVal, threadID);

    int storePtrStatus = pthread_setspecific(valKey, (void*)&myVal);
    if (storePtrStatus != 0) {
        fprintf(stderr, "Error storing pointer to myVal (key: valKey) in thread %ld", threadID);
    }

    printMyVal(threadID);
}


int main() {
    // Creating array of Threads
    pthread_t myThreads[NUM_THREADS];

    // create key value identifying thread local data
    pthread_key_create(&valKey, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Creating thread %d\n", i);

        int createThreadStatus = pthread_create(&myThreads[i], NULL, simpleThreadFunc, (void*)(long)i);
        if (createThreadStatus != 0) {
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
        }
        else {
            printf("Successfully terminated (joined) thread %d\n", i);
        }
    }

    return 0;
}