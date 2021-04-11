#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// this function kills the process that calls this function
void kill_func(int killSignal) {
    printf("Received kill signal %d\n", killSignal);
    printf("\tDying process %d\n", getpid());
    exit(0);
}

void myFunction(int sigVal) {
    printf("Received signal %d\n", sigVal);
    printf("Now you can kill me\n");
    signal(SIGINT, kill_func);
}


int main() {
    // ignore the SIGINT signal (ctrl+c) for 15 seconds
    signal(SIGINT, SIG_IGN);
    
    // for alarm signal call function myFunction
    signal(SIGALRM, myFunction);
    alarm(15);
    
    printf("This gets printed as soon as alarm is called\n");
    
    while (1); // run infinitely
}