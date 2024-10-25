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
    // TODO: split [0, limit) into `parts` ranges, run std::async(std::launch::async, ...) on each, sum the results.
    return 0;
}

int hand_off_value(int value) {
    // TODO: compute value * 2 on another thread and deliver it through a std::promise.
    return 0;
}

bool failure_reaches_caller() {
    // TODO: fail a promise with set_exception on another thread and report whether get() rethrew it.
    return false;
}

bool finishes_within(std::chrono::milliseconds budget, std::chrono::milliseconds work) {
    // TODO: run `work` (a sleep) asynchronously and report whether it completed within `budget`.
    return false;
}

int broadcast_sum(int readers) {
    // TODO: let `readers` threads all wait on one value using std::shared_future, each returning it; sum them.
    return 0;
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
