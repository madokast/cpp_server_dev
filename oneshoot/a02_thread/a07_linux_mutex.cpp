#include<pthread.h> // g++ -lpthread
#include<stdio.h>

#define N 15

typedef struct {
    int cnt;
    pthread_mutex_t* mu;
} atomic_int;


void* adder(void* param) {
    atomic_int* a = (atomic_int*)(param);
    pthread_mutex_lock(a->mu);
    for (int i = 0; i < 1000000 ; ++i) a->cnt += 1;
    printf("[thread] cnt %d\n", a->cnt);
    pthread_mutex_unlock(a->mu);
    return NULL;
}

int main() {
    pthread_mutex_t mu;
    pthread_mutex_init(&mu, NULL);

    atomic_int aint {0, &mu};

    pthread_t threads[N];
    for (int i = 0; i < N ; ++i) pthread_create(&threads[i], NULL, adder, &aint);
    for (int i = 0; i < N ; ++i) pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mu);
    printf("[main] cnt %d\n", aint.cnt);
}