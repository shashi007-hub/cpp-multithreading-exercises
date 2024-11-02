#include <algorithm>
#include <chrono>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

#include "common/check.hpp"

bool is_prime(int n) {
    if (n < 2) {
        return false;
    }
    for (int divisor = 2; divisor * divisor <= n; ++divisor) {
        if (n % divisor == 0) {
            return false;
        }
    }
    return true;
}

int count_primes(int begin, int end) {
    int count = 0;
    for (int n = begin; n < end; ++n) {
        count += is_prime(n);
    }
    return count;
}

int count_primes_async(int limit, int parts) {
    std::vector<std::future<int>> futures;
    const int step = (limit + parts - 1) / parts;
    for (int begin = 0; begin < limit; begin += step) {
        futures.push_back(std::async(std::launch::async, count_primes, begin, std::min(limit, begin + step)));
    }
    int total = 0;
    for (auto& future : futures) {
        total += future.get();
    }
    return total;
}

int hand_off_value(int value) {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread producer([&promise, value] { promise.set_value(value * 2); });
    const int result = future.get();
    producer.join();
    return result;
}

bool failure_reaches_caller() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread producer([&promise] {
        promise.set_exception(std::make_exception_ptr(std::runtime_error("worker failed")));
    });
    bool caught = false;
    try {
        future.get();
    } catch (const std::runtime_error&) {
        caught = true;
    }
    producer.join();
    return caught;
}

bool finishes_within(std::chrono::milliseconds budget, std::chrono::milliseconds work) {
    auto future = std::async(std::launch::async, [work] { std::this_thread::sleep_for(work); });
    return future.wait_for(budget) == std::future_status::ready;
}

int broadcast_sum(int readers) {
    std::promise<int> promise;
    std::shared_future<int> shared = promise.get_future().share();
    std::vector<std::future<int>> consumers;
    for (int i = 0; i < readers; ++i) {
        consumers.push_back(std::async(std::launch::async, [shared] { return shared.get(); }));
    }
    promise.set_value(10);
    int total = 0;
    for (auto& consumer : consumers) {
        total += consumer.get();
    }
    return total;
}

int main() {
    using namespace std::chrono_literals;

    run_test("primes below 200000", [] { CHECK_EQ(count_primes_async(200'000, 8), 17984); });
    run_test("promise hands a value over", [] { CHECK_EQ(hand_off_value(21), 42); });
    run_test("promise carries an exception", [] { CHECK(failure_reaches_caller()); });
    run_test("wait_for detects a slow task", [] {
        CHECK(finishes_within(500ms, 10ms));
        CHECK(!finishes_within(10ms, 200ms));
    });
    run_test("shared_future broadcasts", [] { CHECK_EQ(broadcast_sum(6), 60); });
}
