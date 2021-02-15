#include <stdio.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int x;
    x = 0;
    
    pid_t c_pid = fork();
    
    std::cout << "x before value: " << x << std::endl;
    x++;
    std::cout << "x after value: " << x << std::endl;
    
    if (c_pid == 0 ) {
        // child process
        std:: cout << "I am child process " << getpid() << " with x value: " << x << std::endl;
        exit(0);
    }
    std::cout << "I am parent process " << getpid() << "with child " << c_pid << ". my x is " << x << std::endl;
    exit(0);
}