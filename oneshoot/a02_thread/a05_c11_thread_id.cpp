#include<thread>
#include<iostream>

void run() {
    std::cout << "[thread] thread-id: " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::thread t {run};
    std::cout << "[main] thread-id: " << t.get_id() << std::endl;
    t.join();
}