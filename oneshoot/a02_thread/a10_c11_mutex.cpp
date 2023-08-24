#include<thread>
#include<shared_mutex>
#include<iostream>
#include<vector>

const int N = 15;

void adder(std::shared_mutex* mu, int* cnt) {
    {
        std::unique_lock<std::shared_mutex> guard {*mu}; // 实际传入引用
        for (int i = 0; i < 1000000 ; ++i) ++(*cnt);
    }
    std::shared_lock<std::shared_mutex> guard {*mu};
    std::cout << "[thread] " << *cnt << std::endl;
}

int main() {
    std::shared_mutex mu;
    int cnt {0};

    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) threads.emplace_back(adder, &mu, &cnt);
    for (auto& t : threads) t.join();

    std::cout << "[main] " << cnt << std::endl;
}