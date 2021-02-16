#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>

#include "sharedInfo.h"

#define userInputBufferLength 20

void INThandler(int);
int getUserInputInt(char* const buff, int const buff_len, int* const intVar);
struct info* ctrl;
int id;


int main(void) {
    signal(SIGINT, INThandler);

    // create shared memory segment using shmget()
    // id: file identifier of shared memory segment
    // if key is shared, any process can attach to this shared memory segment
    // 0666 is file permission for shared memory segment
    id = shmget(key, MSIZ, IPC_CREAT | 0666);
    if (id < 0) {
        // error
        fprintf(stderr, "Error with shmget() creating shared memory segment\n");
        exit(1);
    }

    // attach local ptr to shared memory segment created using shmat()
    ctrl = (struct info*)shmat(id, 0, 0);
    if (ctrl <= (struct info*)(0)) {
        // error
        fprintf(stderr, "Error with shmat() attaching to shared memory segment\n");
        // mark the segment to be destroyed
        // segment will actually be destroyed only after last process detaches from it
        shmctl(id, IPC_RMID, NULL);
        exit(1);
    }

    // initial values
    ctrl->value1 = 0;
    ctrl->value2 = 0;
    ctrl->sum = 0;
    ctrl->flag = 0;

    char userInputValue1[userInputBufferLength], userInputValue2[userInputBufferLength], userInputTerminate[userInputBufferLength];

    while (1) {
        // get user input and save to value1, and value2
        printf("\nTerminate? [y/N] ");
        fflush(stdout);

        fgets(userInputTerminate, userInputBufferLength, stdin);
        //// remove \n at end (or can just do it in comparison)
        //userInputTerminate[strlen(userInputTerminate) - 1] = '\0';

        if (strcmp(userInputTerminate, "y\n") == 0) {
            // set ctrl->flag to -1 to tell processB to also terminate
            ctrl->flag = -1;

            break;
        }


        printf("Enter value1: ");
        fflush(stdout);

        int value1 = 0;
        if (getUserInputInt(userInputValue1, userInputBufferLength, &value1) == -1) {
            continue;
        }
        else {
            ctrl->value1 = value1;
        }


        printf("Enter value2: ");
        fflush(stdout);

        int value2;
        if (getUserInputInt(userInputValue2, userInputBufferLength, &value2) == -1) {
            continue;
        }
        else {
            ctrl->value2 = value2;
        }

        // when processA done, sets flag to 1, telling processB it can proceed
        ctrl->flag = 1;
    }

    // When finished
    // shmdt(): detaches shared memory segment at ctrl  from the address space of the calling process
    shmdt(ctrl);

    // mark the segment to be destroyed
    // segment will actually be destroyed only after last process detaches from it
    shmctl(id, IPC_RMID, NULL);

    exit(0);
}


int getUserInputInt(char* const buff, int const buff_len, int* const intVar) {
    /* Parameters:
        buff (ptr to user input buffer)
        buff_len (length of user input buffer)
        intVar (ptr to intVar where final result would be saved if user entered good int,
            otherwise intVar won't be changed if bad int
        )

        returns -1 if user entered bad int
        returns 0 if user entered good int
     */


    char* end = NULL;

    fgets(buff, buff_len, stdin);
    // remove \n character than fgets keeps
    buff[strlen(buff) - 1] = '\0';

    errno = 0;
    const long longVar = strtol(buff, &end, 10);

    // if ((end == buff) || ('\0' != *end) || ((LONG_MIN == longVar || LONG_MAX == longVar) && ERANGE == errno) || (longVar > INT_MAX) || (longVar < INT_MIN)) {
    if (end == buff) {
        fprintf(stderr, "%s: not a decimal number\n", buff);
        return -1;
    }
    else if ('\0' != *end) {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
        return -1;
    }
    else if ((LONG_MIN == longVar || LONG_MAX == longVar) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", buff);
        return -1;
    }
    else if (longVar > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", longVar);
        return -1;
    }
    else if (longVar < INT_MIN) {
        fprintf(stderr, "%ld less than INT_MIN\n", longVar);
        return -1;
    }
    else {
        // set the intVar to valid user input
        *intVar = (int)longVar;
        return 0;
    }
}

void INThandler(int sig) {
    // shmdt(): detaches shared memory segment at ctrl  from the address space of the calling process
    shmdt(ctrl);

    // mark the segment to be destroyed
    // segment will actually be destroyed only after last process detaches from it
    shmctl(id, IPC_RMID, NULL);

    exit(0);
}
// can use `ipcs` to monitor created shared memory segments