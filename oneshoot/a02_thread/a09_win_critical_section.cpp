#include<windows.h>
#include<stdio.h>

#define N 15

typedef struct {
    int cnt;
    LPCRITICAL_SECTION cs;
} atomic_int;


DWORD CALLBACK adder(LPVOID param) {
    atomic_int* a = (atomic_int*)(param);
    EnterCriticalSection(a->cs);
    for (int i = 0; i < 1000000 ; ++i) a->cnt += 1;
    printf("[thread] cnt %d\n", a->cnt);
    LeaveCriticalSection(a->cs);
    return 0;
}

int main() {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    atomic_int aint {0, &cs};
    HANDLE threads[N];
    for (int i = 0; i < N ; ++i) threads[i] = CreateThread(NULL, 0, adder, &aint, 0, NULL);
    for (int i = 0; i < N ; ++i) WaitForSingleObject(threads[i], INFINITE);

    DeleteCriticalSection(&cs);
    printf("[main] cnt %d\n", aint.cnt);
}