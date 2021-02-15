#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Need one argument that is number of child processes  %d.\n", argc);
        exit(1);
    }

    std::string arg1_s = argv[1];
    int n = stoi(arg1_s);
    printf("%d\n", n);

    if (n % 2 != 0) {
        printf("Number is odd!\n");
        exit(1);
    }

    printf("PARENT: %d\n", getpid());

    for (int i = 0; i < n - 1; i++) {
        pid_t PID = fork();
        if (PID == 0) {
            printf("CHILD %d: %d\n", i+1, getpid());
            fflush(stdout);
        }
        else {
            break;
        }
    }
}