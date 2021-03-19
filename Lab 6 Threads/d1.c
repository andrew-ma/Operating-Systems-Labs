#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

void* printHello(void* threadId) {
    printf("\n%ld:Hello World!\n", (long)threadId);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    for (int t=0; t<NUM_THREADS; ++t) {
        printf("Creating thread %d\n", t);
        int rc = pthread_create(&threads[t], NULL, printHello, (void*)(long) t);
        
        if (rc != 0) {
            printf("ERROR: Return Code from pthread_create() is %d\n", rc);
        }
    }
    
    pthread_exit(NULL);
}