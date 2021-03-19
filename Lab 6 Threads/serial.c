/* Serial */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>

#define NUM_TESTS 10

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: program <Number Of Intervals>\n");
        return 0;
    }

    char* end;

    errno = 0;
    unsigned long const NUM_INTERVALS = strtoul(argv[1], &end, 10);

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

    unsigned char const UPPER_BOUND = 1;
    unsigned char const LOWER_BOUND = 0;

    // Setup Timer
    struct timeval startTime, stopTime;

    long long totalTestMicroseconds = 0;

    // loop NUM_TESTS times to and get average elapsed time
    for (int i = 0; i < NUM_TESTS; ++i) {

        gettimeofday(&startTime, NULL);

        double const EACH_RECTANGLE_WIDTH = ((double)UPPER_BOUND - LOWER_BOUND) / NUM_INTERVALS;

        double sum = 0.0;

        for (int i = 0; i < NUM_INTERVALS; ++i) {
            double const x = (double)i * EACH_RECTANGLE_WIDTH + LOWER_BOUND;
            sum += 4.0 * (EACH_RECTANGLE_WIDTH * (sqrt(1 - x * x)));
        }

        // Stop Timer
        gettimeofday(&stopTime, NULL);

        // printf("Sum %f\n", sum);

        long long elapsedMicroseconds = (long long)(stopTime.tv_usec - startTime.tv_usec) + (long long)((stopTime.tv_sec - startTime.tv_sec) * 1000000);
        // printf("Time: %lld microseconds\n", elapsedMicroseconds);

        totalTestMicroseconds += elapsedMicroseconds;
    }

    printf("Average Microseconds: %lld\n", totalTestMicroseconds / NUM_TESTS);

    return 0;
}