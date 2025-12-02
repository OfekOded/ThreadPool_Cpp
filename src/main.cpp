#include <iostream>
#include <chrono>
#include "ThreadPool.h"

int main() {
    ThreadPool pool(4);

    auto future1 = pool.enqueue([](int x) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return x * x;
        }, 10);

    auto future2 = pool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return std::string("Hello from ThreadPool!");
        });

    std::cout << "Waiting for results..." << std::endl;

    std::cout << "Result 1: " << future1.get() << std::endl;
    std::cout << "Result 2: " << future2.get() << std::endl;

    return 0;
}