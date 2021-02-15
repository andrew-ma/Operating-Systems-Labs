#include <cstdio>
#include <unistd.h>

int main() {
    int val = 0;
    pid_t PID = fork();
    if (PID == 0) {
        //child process
        val += 2;
        printf("CHILD:  VAL: %d, PID: %d\n", val, getpid());
        
    } else {
        val += 5;
        printf("PARENT:  VAL: %d, PID: %d\n", val, getpid());
        
    }
}
// val is isolated between processes and ends up being 2 in the child and 5 in the parent.