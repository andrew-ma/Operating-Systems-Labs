#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("I am Parent\n"); // once
    fork(); // 2 processes: parent, child
    printf("This is by a parent and child\n"); // twice
    fork(); // 4 processes: parent, child1, child2, grandchild1
    printf("This should be 4 times\n");
    
    printf("%d\n", getpid());
}