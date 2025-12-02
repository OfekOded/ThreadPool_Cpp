#include <gtest/gtest.h>
#include "ThreadPool.h"

TEST(ThreadPoolBasic, SimpleTask) {
    ThreadPool pool(2);

    auto result = pool.enqueue([] { return 42; });

    ASSERT_EQ(result.get(), 42);
}

TEST(ThreadPoolBasic, MultipleTasks) {
    ThreadPool pool(4);
    std::vector<std::future<int>> results;

    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.enqueue([i] {
            return i * i;
            }));
    }

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(results[i].get(), i * i);
    }
}