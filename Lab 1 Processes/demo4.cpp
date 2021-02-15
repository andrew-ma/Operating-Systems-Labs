#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t c_pid;
    c_pid = fork();
    if (c_pid == 0) {
        //child
        printf("I am child, id is %d\n", getpid());
        printf("I am child, parent is %d\n", getppid());
        printf("Child is sleeping for 10 seconds until when the parent might be dead already\n");
        sleep(10); // after 10 seconds, prints and exits
        printf("I am the same child with Id %d, but my parent id is %d", getpid(), getppid());
        
    } else {
        // not child
        printf("I am the parent with id %d. My parent is %d and child is %d\n", getpid(), getppid(), c_pid);
        sleep(5); //exits after 5 seconds
    }
}