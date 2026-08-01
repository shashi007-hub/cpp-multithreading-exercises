#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <stop_token>
#include <thread>
#include <vector>

#include "common/check.hpp"

class PeriodicTask {
public:
    PeriodicTask(std::chrono::milliseconds period, std::function<void()> task)
        : worker_([period, task = std::move(task)](std::stop_token stop) { run(stop, period, task); }) {}

    ~PeriodicTask() { worker_.request_stop(); }

private:
    static void run(std::stop_token stop, std::chrono::milliseconds period, const std::function<void()>& task) {
        // TODO: sleep for `period` in a way that a stop request interrupts immediately, then run `task`.
        // Hint: std::condition_variable_any::wait_for accepts a std::stop_token.
    }

    std::jthread worker_;
};

std::optional<std::size_t> parallel_find(const std::vector<int>& values, int target, unsigned workers) {
    std::stop_source found;
    std::atomic<std::size_t> position{values.size()};
    std::vector<std::jthread> threads;
    const std::size_t chunk = (values.size() + workers - 1) / workers;

    for (unsigned worker = 0; worker < workers; ++worker) {
        threads.emplace_back([&, worker] {
            const std::size_t begin = std::min(values.size(), worker * chunk);
            const std::size_t end = std::min(values.size(), begin + chunk);
            for (std::size_t i = begin; i < end; ++i) {
                // TODO: stop scanning once another worker has found the target; otherwise record the hit
                // and ask everyone else to stop through the shared std::stop_source.
            }
        });
    }
    threads.clear();

    if (position == values.size()) {
        return std::nullopt;
    }
    return position.load();
}

int main() {
    using namespace std::chrono_literals;

    run_test("periodic task ticks until destroyed", [] {
        std::atomic<int> ticks{0};
        {
            PeriodicTask task(10ms, [&] { ++ticks; });
            std::this_thread::sleep_for(105ms);
        }
        const int after_destruction = ticks.load();
        CHECK(after_destruction >= 5);
        CHECK(after_destruction <= 12);
        std::this_thread::sleep_for(50ms);
        CHECK_EQ(ticks.load(), after_destruction);
    });

    run_test("destruction interrupts a long sleep", [] {
        const auto start = std::chrono::steady_clock::now();
        {
            PeriodicTask task(1h, [] {});
            std::this_thread::sleep_for(20ms);
        }
        CHECK(std::chrono::steady_clock::now() - start < 1s);
    });

    run_test("parallel_find locates the unique target", [] {
        std::vector<int> values(5'000'000, 0);
        values[3'777'777] = 1;
        CHECK_EQ(parallel_find(values, 1, 4).value(), 3'777'777u);
        CHECK(!parallel_find(values, 2, 4).has_value());
    });
}
