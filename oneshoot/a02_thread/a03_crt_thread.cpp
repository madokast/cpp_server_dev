#include<process.h>
#include<windows.h>
#include<stdio.h>

unsigned int __stdcall run(void* times) {
    for (int i = 0; i < *(static_cast<int*>(times)); ++i) printf("thread run\n");
    return 0;
}

int main() {
    unsigned int threadId;
    int times = 3;
    uintptr_t tHandle = _beginthreadex(NULL, 0, run, &times, 0, &threadId);

    printf("main run\n");
    printf("main run\n");

    WaitForSingleObject(reinterpret_cast<void*>(tHandle), INFINITE);
    CloseHandle(reinterpret_cast<void*>(tHandle));
}