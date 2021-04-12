/* test4.c */
#include <stdlib.h>

int main() {
    char* x = (char*) malloc(10);
    x[10] = 'a';
    return 0;
}