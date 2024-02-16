#pragma once

#include <thread>
#include <vector>

template <typename Task>
void run_concurrently(unsigned count, Task&& task) {
    std::vector<std::jthread> threads;
    threads.reserve(count);
    for (unsigned index = 0; index < count; ++index) {
        threads.emplace_back([&task, index] { task(index); });
    }
}
