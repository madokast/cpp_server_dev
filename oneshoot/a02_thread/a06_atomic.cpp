#include <iostream>
#include <thread>
#include <atomic>
int main() {
    std::atomic<int> cnt {0};
    std::thread t1{[&cnt]{
        for (int i {0}; i < 10; ++i) cnt+=1;
    }};
    std::thread t2{[&cnt]{
        for (int i {0}; i < 10; ++i) cnt+=1;
    }};
    t1.join();
    t2.join();
    std::cout << cnt.load() << std::endl;
}