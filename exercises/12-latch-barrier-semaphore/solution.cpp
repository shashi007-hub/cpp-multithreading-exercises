#include <atomic>
#include <barrier>
#include <chrono>
#include <cstddef>
#include <latch>
#include <semaphore>
#include <thread>
#include <vector>

#include "common/check.hpp"
#include "common/concurrent.hpp"

int workers_released_together(unsigned workers) {
    std::latch start_gate(1);
    std::atomic<int> started{0};
    std::vector<std::jthread> threads;
    for (unsigned i = 0; i < workers; ++i) {
        threads.emplace_back([&] {
            start_gate.wait();
            ++started;
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    const int before_open = started.load();
    start_gate.count_down();
    threads.clear();
    return before_open == 0 ? started.load() : -1;
}

bool neighbours_see_completed_phase(unsigned workers, int phases) {
    std::vector<int> values(workers, 0);
    std::atomic<bool> consistent{true};
    std::barrier sync_point(workers);
    run_concurrently(workers, [&](unsigned id) {
        for (int phase = 1; phase <= phases; ++phase) {
            values[id] = phase;
            sync_point.arrive_and_wait();
            if (values[(id + 1) % workers] != phase) {
                consistent = false;
            }
            sync_point.arrive_and_wait();
        }
    });
    return consistent;
}

int max_concurrency(unsigned tasks, std::ptrdiff_t limit) {
    std::counting_semaphore<64> permits(limit);
    std::atomic<int> running{0};
    std::atomic<int> peak{0};
    run_concurrently(tasks, [&](unsigned) {
        permits.acquire();
        const int now = ++running;
        int seen = peak.load();
        while (now > seen && !peak.compare_exchange_weak(seen, now)) {
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        --running;
        permits.release();
    });
    return peak;
}

int main() {
    run_test("latch releases all workers at once", [] { CHECK_EQ(workers_released_together(8), 8); });
    run_test("barrier separates write and read phases", [] { CHECK(neighbours_see_completed_phase(6, 500)); });
    run_test("semaphore caps concurrency", [] {
        const int peak = max_concurrency(16, 3);
        CHECK(peak <= 3);
        CHECK(peak >= 2);
    });
}
