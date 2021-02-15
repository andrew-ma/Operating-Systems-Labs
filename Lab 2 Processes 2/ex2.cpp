#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>


int subtract(int num1, int num2) {
    return num1 - num2;
}

int main() {
    int num1 = 10;
    int num2 = 5;

    //child1
    pid_t PID = fork();
    if (PID == 0) {
        //child1
        int subtractResult = subtract(num1, num2);
        printf("CHILD1: %d, child1's PID: %d\n", subtractResult, getpid());

        //child2
        PID = fork();
        if (PID == 0) {
            //child2
            printf("CHILD2: %d, child2's PID: %d\n", num1 + num2, getpid()); //add two numbers
            exit(0);
        } else {
            //child1
            wait(nullptr); // child1 waits for child2 to die
            printf("CHILD1: %d\n", num1 * num2); //multiply 2 numbers
            exit(0);                       //child1 terminate
        }
    } else {
        //parent
        printf("PARENT: parent's PID: %d\n", getpid()); // print parent process's id
        wait(nullptr);                                        //wait for child1 to die
        exit(0);                                        // resume and terminate program
    }
    fflush(stdout);
}