#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> taskQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool stopFlag;

public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {

        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);
            if (stopFlag)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            taskQueue.emplace([task]() { (*task)(); });
        }

        cv.notify_one();

        return res;
    }
};