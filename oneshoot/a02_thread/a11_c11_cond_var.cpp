#include<thread>
#include<mutex>
#include<condition_variable>
#include<deque>
#include<vector>
#include<iostream>

struct factory {
    std::mutex& mu;
    std::condition_variable& consumers;
    std::condition_variable& producers;
    std::deque<int>& queue;
};

void consumer(factory f) {
    std::unique_lock<std::mutex> guard {f.mu};
    while (1) {
        while (f.queue.empty()) {
            f.producers.notify_one();
            f.consumers.wait(guard); // 挂起并释放 guard
        }
        while (!f.queue.empty()) {
            std::cout << "consume-" << std::this_thread::get_id() << " " << f.queue.front() << std::endl;
            f.queue.pop_front();
        }
    }
}

void producer(factory f) {
    std::unique_lock<std::mutex> guard {f.mu};
    while (1) {
        while (!f.queue.empty()) {
            f.consumers.notify_one();
            f.producers.wait(guard); // 挂起并释放 guard
        }
        if (f.queue.empty()) {
            for (int i = 0; i < std::rand() % 3 + 1 ; ++i) f.queue.push_back(std::rand());
            std::cout << "producer-" << std::this_thread::get_id() << std::endl;
        }
    }
}

int main() {
    std::mutex mu;
    std::condition_variable consumers;
    std::condition_variable producers;
    std::deque<int> queue;

    factory f {mu, consumers, producers, queue};

    std::vector<std::thread> ts;
    for (int i = 0 ; i < 1000 ; ++i) ts.emplace_back(consumer, f);
    for (int i = 0 ; i < 1 ; ++i) ts.emplace_back(producer, f);

    for (auto& t : ts) t.join();
}