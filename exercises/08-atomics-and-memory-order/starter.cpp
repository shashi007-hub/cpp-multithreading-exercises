#include <atomic>
#include <mutex>
#include <thread>

#include "common/check.hpp"
#include "common/concurrent.hpp"

class SpinLock {
public:
    void lock() {
        // TODO: spin on flag_.test_and_set with the right memory order.
    }

    void unlock() {
        // TODO: release the flag.
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

void update_max(std::atomic<int>& target, int candidate) {
    // TODO: raise target to candidate if it is larger, using a compare_exchange loop.
}

struct Message {
    int payload = 0;
    std::atomic<bool> ready{false};

    void publish(int value) {
        // TODO: write the payload, then publish it with a release store.
    }

    int wait_and_read() const {
        // TODO: spin with an acquire load, then read the payload.
        return 0;
    }
};

int main() {
    run_test("spin lock gives mutual exclusion", [] {
        SpinLock lock;
        long long counter = 0;
        run_concurrently(4, [&](unsigned) {
            for (int i = 0; i < 100'000; ++i) {
                std::lock_guard guard(lock);
                ++counter;
            }
        });
        CHECK_EQ(counter, 400'000);
    });

    run_test("update_max keeps the largest value", [] {
        std::atomic<int> maximum{0};
        run_concurrently(8, [&](unsigned id) {
            for (int i = 0; i < 10'000; ++i) {
                update_max(maximum, static_cast<int>(id) * 10'000 + i);
            }
        });
        CHECK_EQ(maximum.load(), 79'999);
    });

    run_test("release/acquire publishes a payload", [] {
        for (int round = 0; round < 1000; ++round) {
            Message message;
            int seen = 0;
            std::thread reader([&] { seen = message.wait_and_read(); });
            message.publish(round + 1);
            reader.join();
            CHECK_EQ(seen, round + 1);
        }
    });
}
