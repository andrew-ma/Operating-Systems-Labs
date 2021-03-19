#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5

void* simpleThreadFunc(void* threadID) {
    long threadID_num = (long)threadID;
    printf("My Id is %ld\n", threadID_num);
    
    // exit value will be used in void** retVal for pthread_join()
    pthread_exit((void*)(threadID_num * 2));
}

int main() {
    // Declaring array of threads
    pthread_t myThreads[NUM_THREADS];
    int returnValues[NUM_THREADS];


    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Creating thread no. %d, and sending ID %d\n", i, i);
        int status = pthread_create(&myThreads[i], NULL, simpleThreadFunc, (void*)(long)i);

        if (status != 0) {
            // error
            fprintf(stderr, "Error creating thread %d\n", i);
            return -1;
        }
        else {
            printf("Successful creating thread %d\n", i);
        }
    }


    for (int i = 0; i < NUM_THREADS; ++i) {
        int retStatus = pthread_join(myThreads[i], (void**)(&returnValues[i]));

        if (retStatus != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return -1;
        }
        else {
            printf("Successful terminating (joining) thread %d\n", i);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        printf("Thread %d: %d\n", i, returnValues[i]);
    }

    return 0;
}