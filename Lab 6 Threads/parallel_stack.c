/* Parallel */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>

#define NUM_TESTS 10

unsigned char const UPPER_BOUND = 1;
unsigned char const LOWER_BOUND = 0;
double EACH_RECTANGLE_WIDTH;


struct ThreadArgument {
    unsigned long start;
    unsigned long end;
    double sum;
};

void* computeIntegral(void* argument) {
    struct ThreadArgument* arg = (struct ThreadArgument*)argument;

    arg->sum = 0.0;
    for (unsigned long i = arg->start; i < arg->end; ++i) {
        double const x = (double)i * EACH_RECTANGLE_WIDTH + LOWER_BOUND;
        arg->sum += 4.0 * (EACH_RECTANGLE_WIDTH * (sqrt(1 - x * x)));
    }
    pthread_exit(NULL);

}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: program <Number Of Intervals> <Number Of Threads>\n");
        return 0;
    }

    char* end;

    errno = 0;
    long const NUM_INTERVALS = strtoul(argv[1], &end, 10);

    if (end == argv[1]) {
        fprintf(stderr, "%s: not a decimal number\n", argv[1]);
        return -1;
    }
    else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", argv[1], end);
        return -1;
    }
    else if ((0 == NUM_INTERVALS || ULONG_MAX == NUM_INTERVALS) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type unsigned long\n", argv[1]);
        return -1;
    }
    else if (NUM_INTERVALS == 0) {
        fprintf(stderr, "Can't have zero intervals\n");
        return -1;
    }


    errno = 0;
    unsigned long const NUM_THREADS = strtoul(argv[2], &end, 10);

    if (end == argv[2]) {
        fprintf(stderr, "%s: not a decimal number\n", argv[2]);
        return -1;
    }
    else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", argv[2], end);
        return -1;
    }
    else if ((0 == NUM_THREADS || ULONG_MAX == NUM_THREADS) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type unsigned long\n", argv[2]);
        return -1;
    }

    // Setup Timer
    struct timeval startTime, stopTime;

    long long totalTestMicroseconds = 0;

    // loop NUM_TESTS times to and get average elapsed time
    for (int i = 0; i < NUM_TESTS; ++i) {

        gettimeofday(&startTime, NULL);


        // Create Array of pthreads
        pthread_t myThreads[NUM_THREADS];

        // the ThreadArgument structures are allocated in the main stack, so we can address (ptr) the structures within thread functions
        struct ThreadArgument threadArguments[NUM_THREADS];


        EACH_RECTANGLE_WIDTH = ((double)UPPER_BOUND - LOWER_BOUND) / NUM_INTERVALS;


        unsigned long const INTERVALS_PER_THREAD = ((double)NUM_INTERVALS / NUM_THREADS);
        unsigned long const LEFTOVER_INTERVALS_FOR_LAST_THREAD = NUM_INTERVALS % NUM_THREADS;

        // printf("Intervals per thread %ld, and leftover %ld\n", INTERVALS_PER_THREAD, LEFTOVER_INTERVALS_FOR_LAST_THREAD);

        /* Creating Threads */
        for (int i = 0; i < NUM_THREADS; ++i) {
            unsigned long start = i * INTERVALS_PER_THREAD;
            unsigned long end = start + INTERVALS_PER_THREAD - 1;

            if (i == NUM_THREADS - 1) {
                // Add leftovers to last thread
                end += LEFTOVER_INTERVALS_FOR_LAST_THREAD;
            }

            // printf("Start %lu, End %lu\n", start, end);

            threadArguments[i] = (struct ThreadArgument){ .start = start, .end = end, .sum = 0.0 };

            int createThreadStatus = pthread_create(&myThreads[i], NULL, computeIntegral, (void*)&threadArguments[i]);
            if (createThreadStatus != 0) {
                fprintf(stderr, "Error creating thread %d\n", i);
            }
        }


        double totalSum = 0.0;

        // /* Joining Threads */
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(myThreads[i], NULL);

            totalSum += threadArguments[i].sum;

            // printf("Thread %d: %f\n", i, threadArguments[i].sum);
        }

        // Stop Timer
        gettimeofday(&stopTime, NULL);

        // printf("Sum: %f\n", totalSum);

        long long elapsedMicroseconds = (long long)(stopTime.tv_usec - startTime.tv_usec) + (long long)((stopTime.tv_sec - startTime.tv_sec) * 1000000);

        // printf("Time: %lld microseconds\n", elapsedMicroseconds);

        totalTestMicroseconds += elapsedMicroseconds;
    }
    
    printf("Average Microseconds: %lld\n", totalTestMicroseconds / NUM_TESTS);

    return 0;
}