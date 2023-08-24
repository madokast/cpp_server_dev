#include<windows.h>
#include<stdio.h>

DWORD WINAPI run(LPVOID times) {
    for (int i = 0; i < *(static_cast<int*>(times)); ++i) printf("thread run\n");
    return 0;
}

int main() {
    DWORD threadId;
    int times = 3;
    HANDLE Hthread = CreateThread(NULL, 9, run, &times, 0, &threadId);

    printf("main run\n");
    printf("main run\n");

    WaitForSingleObject(Hthread, INFINITE);
}