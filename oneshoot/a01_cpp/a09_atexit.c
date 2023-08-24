#include<stdio.h>
#include<stdlib.h>

void _atexit(void) {printf("receive exit signal\n");}

int main() {
    atexit(_atexit);
    exit(0);
}