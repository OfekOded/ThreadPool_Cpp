#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stopFlag(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] {
                        return stopFlag || !taskQueue.empty();
                        });

                    if (stopFlag && taskQueue.empty())
                        return;

                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                task();
            }
            });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stopFlag = true;
    }
    cv.notify_all();
    for (std::thread& worker : threads) {
        if (worker.joinable())
            worker.join();
    }
}
