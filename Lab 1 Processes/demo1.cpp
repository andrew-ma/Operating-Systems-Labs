#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    pid_t c_pid;
    
    std::cout << "The pid of the parent is " << getpid() << std::endl;
    
    c_pid = fork();
    
    if (c_pid == 0) {
        // child process
        std::cout << "I am the child with '" << getpid() << "'. My parent was " << getppid() << std::endl;
        std:: cout << "fork return value is " << c_pid << std::endl;
        exit(0);
    } else {
    
    std::cout << "I am the parent with '" << getpid() << "'. My child was " << c_pid << std::endl;
    exit(0);
    }
}