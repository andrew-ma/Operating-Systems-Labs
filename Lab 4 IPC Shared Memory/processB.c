#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "sharedInfo.h"


int main(void) {
    // get id of already created shared memory segment
    int const id = shmget(key, MSIZ, 0);
    if (id < 0) {
        // error
        fprintf(stderr, "Error with shmget() getting shared memory segment\n");
        exit(1);
    }

    //attach local ptr to shared memory
    struct info* ctrl = (struct info*)shmat(id, 0, 0);
    if (ctrl <= (struct info*)(0)) {
        // error
        fprintf(stderr, "Error with shmat() attaching to shared memory segment\n");
        exit(1);
    }

    while (1) {
        // wait for processA to set flag to 1 for done changing value1 and value2
        while (ctrl->flag != 1 && ctrl->flag != -1);

        if (ctrl->flag == -1) {
            // if processA set flag to -1, then we want to exit after cleanup
            break;
        }

        ctrl->sum = ctrl->value1 + ctrl->value2;
        // display result
        printf("Sum: %d\n", ctrl->sum);
        fflush(stdout);

        // setting flag to 0
        ctrl->flag = 0;
    }


    // detach ptr from shared memory
    shmdt(ctrl);
    exit(0);
}