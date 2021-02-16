#ifndef INFO_H
#define INFO_H

#include <sys/types.h>

struct info {
    char flag; // set by pA (if -1, then both processes terminate)
    int value1, value2; // set by pA
    int sum; // set by pB, and displayed by pB
};

key_t key = 12345;
size_t MSIZ = sizeof(struct info);

#endif