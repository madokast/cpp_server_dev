#include<thread>
#include<mutex>
#include<iostream>
#include<vector>
const int N = 15;

void adder(std::mutex& mu, int& cnt) {
    std::lock_guard<std::mutex> guard {mu};
    for (int i = 0; i < 1000000 ; ++i) ++cnt;
    std::cout << "[thread] " << cnt << std::endl;
}

int main() {
    std::mutex mu;
    int cnt {0};

    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) threads.emplace_back(adder, std::ref(mu), std::ref(cnt));
    for (auto& t : threads) t.join();

    std::cout << "[main] " << cnt << std::endl;
}