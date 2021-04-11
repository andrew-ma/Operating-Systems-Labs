#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t child_pid = -1;
int const NUM_SECONDS_TIL_RESTORE = 10;

void child_sigint_msg_handler(int signum) {
    printf("I am the child [%d], and I can't be killed yet\n", getpid());
}

void child_sigusr1_handler(int signum) {
    printf("Child [%d] has received signal to terminate from parent\n", getpid());
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT); // child kills itself
}

void child_ignore_sigint(int signum) {
    signal(SIGINT, SIG_IGN);
}

void parent_sigint_msg_handler(int signum) {
    printf("System is protected. I am the parent [%d] and I can't be killed yet\n", getpid());
}


void parent_sigchld_handler(int signum) {
    // when child terminates, it sends SIGCHLD
    // so now parent can print goodbye message and terminate
        
    printf("%s", "Goodbye\n");
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT); // parent kills itself
}


void parent_sigint_goodbye_handler(int signum) {
    signal(SIGCHLD, parent_sigchld_handler); // when child terminates, it sends SIGCHLD, so associate that with custom sigchld handler
    
    // send signal to child
    kill(child_pid, SIGUSR1);
    
    printf("%s", "Parent trying to terminate the child process and has sent signal SIGUSR1\n");

}

void restore_default_sigint(int signum) {
    printf("Passed %d seconds\n", NUM_SECONDS_TIL_RESTORE);
    
    // change parent's SIGINT handler to goodbye function
    signal(SIGINT, parent_sigint_goodbye_handler);
}


int main() {
    // create child process
    child_pid = fork();
    if (child_pid == 0) {
        // child process
        
        signal(SIGINT, child_sigint_msg_handler); // child catches SIGINT signal and prints message
        signal(SIGUSR1, child_sigusr1_handler); // child catches SIGUSR1 signal and terminates
        
        // change Alarm signal handler to call function that will reset the SIGINT signal handler to Ignore
        signal(SIGALRM, child_ignore_sigint);
        // Alarm after 10 seconds to change SIGINT handler to default
        alarm(NUM_SECONDS_TIL_RESTORE);
        
    } else {
        // parent process
        
        signal(SIGINT, parent_sigint_msg_handler); // parent catches SIGINT signal (Ctrl+C) and prints message that system is protected
    
        // change Alarm signal handler to call function that will reset the SIGINT signal handler back to default
        signal(SIGALRM, restore_default_sigint);
        // Alarm after 10 seconds to change SIGINT handler to default
        alarm(NUM_SECONDS_TIL_RESTORE);
    }
    
    
    while(1);
}