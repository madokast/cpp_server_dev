#include<pthread.h>
#include<stdio.h>
void* run(void* times) {
    for (int i = 0; i < *(static_cast<int*>(times)); ++i) printf("thread run\n");
    return NULL;
}

// g++ 编译时加上 -lpthread 链接 pthread 库
int main() {
    pthread_t tid;
    int times = 3;
    pthread_create(&tid, NULL, run, &times);

    printf("main run\n");
    printf("main run\n");
    pthread_join(tid, NULL);
}