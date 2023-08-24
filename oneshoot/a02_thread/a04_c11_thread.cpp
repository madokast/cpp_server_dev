#include<thread>
#include<stdio.h>

void run(int times) {
    for (int i = 0; i < times; ++i) printf("thread run\n");
}

int main() {
    std::thread t {run, 3};

    printf("main run\n");
    printf("main run\n");

    t.join();
}