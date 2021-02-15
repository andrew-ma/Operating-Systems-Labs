#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main() {
    // orphan process - parent ends before child
    // notice how the parent PID changes after the child sleeps
    // this is because as soon as the parent dies and the child becomes an orphan it is adopted by the process dispatcher
    printf("About to create an orphan process that is still sleeping when the parent ends\n");
    pid_t PID = fork();
    if (PID == 0) {
        printf("Child %d\n", getpid());
        sleep(20);
        printf("After sleep Parent %d\n", getppid());


        // zombie process - child ends before parent without parent knowing
        // when the parent is sleeping, the child ends and becomes a zombie
        // we can see a process is a zombie by typing 'ps -el' and seeing 'Z' in the second column
        printf("About to create a child that ends when the parent is still sleeping\n");
        if (fork() != 0) {
            // parent
            printf("Parent\n");
            sleep(10);
        } else {
            // child does nothing
        }



        // sleeping beauty process - process sleeps
        printf("About to Sleep process: %d\n", getpid());
        sleep(10);
        printf("Awake:  %d\n", getpid());
    } else {
        printf("Original Parent %d\n", getpid());
    }

}