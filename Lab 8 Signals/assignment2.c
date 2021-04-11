#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int const LENGTH = 26;

void sigint_handler(int signum) {
    // print page full of gibberish message
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                        'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        'o', 'p', 'q', 'r', 's', 't', 'u',
                        'v', 'w', 'x', 'y', 'z'};
    for (int i=0; i < 10000; ++i) {
        printf("%c", alphabet[rand() % LENGTH]);
    }
    printf("%s", "\n");
}

void restore_sigint_handler(int signum) {
    signal(SIGINT, SIG_DFL);
}

int main() {
    signal(SIGINT, sigint_handler);
    
    signal(SIGALRM, restore_sigint_handler);
    alarm(10);
    
    while (1);
}