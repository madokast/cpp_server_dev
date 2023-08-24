#include<pthread.h>
#include<semaphore.h>
#include <unistd.h> // sleep
#include<stdio.h>

#define CUR 3
#define TOTAL 10

void* run(void* p) {
    sem_t* s = (sem_t*) p;
    sem_wait(s);
    for (int i = 0; i < 3 ; ++i) {
        printf("thread %lu >> %d\n", pthread_self(), i);
        sleep(1);
    }
    sem_post(s);
    return NULL;
}


int main() {
    sem_t s;
    sem_init(&s, 0, CUR);

    pthread_t threads[TOTAL];
    for (int i = 0; i < TOTAL ; ++i) pthread_create(&threads[i], NULL, run, &s);
    for (int i = 0; i < TOTAL ; ++i) pthread_join(threads[i], NULL);

    sem_destroy(&s);
}