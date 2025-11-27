#include <iostream>
#include <chrono>
#include <string>
#include <mutex>
#include <thread>
#include "ThreadPool.h"

std::mutex printMtx;

void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(printMtx);
    std::cout << msg << std::endl;
}

int main() {
    safePrint("--- Main: Starting ThreadPool with 4 threads ---");

    ThreadPool pool(4);

    for (int i = 1; i <= 8; ++i) {
        pool.enqueue([i] {
            safePrint("Task " + std::to_string(i) + " started working...");

            std::this_thread::sleep_for(std::chrono::seconds(1));

            safePrint("Task " + std::to_string(i) + " DONE.");
            });
    }

    safePrint("--- Main: All tasks submitted. Waiting a bit... ---");

    std::this_thread::sleep_for(std::chrono::seconds(5));

    safePrint("--- Main: Exiting (Destructor will clean up) ---");
    return 0;
}