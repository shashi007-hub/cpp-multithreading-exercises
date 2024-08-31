#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include "common/check.hpp"
#include "common/concurrent.hpp"

template <typename T>
class BlockingQueue {
public:
    bool push(T value) {
        // TODO: reject pushes after close(); otherwise enqueue and wake one consumer.
        return false;
    }

    std::optional<T> pop() {
        // TODO: wait for an item or close(); return nullopt only once closed AND drained.
        return std::nullopt;
    }

    void close() {
        // TODO: mark closed and wake every waiting consumer.
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::deque<T> items_;
    bool closed_ = false;
};

int main() {
    run_test("consumers drain everything after close", [] {
        BlockingQueue<int> queue;
        std::atomic<long long> sum{0};
        std::atomic<int> count{0};
        std::vector<std::jthread> consumers;
        for (int i = 0; i < 4; ++i) {
            consumers.emplace_back([&] {
                while (auto item = queue.pop()) {
                    sum += *item;
                    ++count;
                }
            });
        }
        run_concurrently(4, [&](unsigned) {
            for (int i = 1; i <= 2500; ++i) {
                queue.push(i);
            }
        });
        queue.close();
        consumers.clear();
        CHECK_EQ(count.load(), 10000);
        CHECK_EQ(sum.load(), 4LL * 2500 * 2501 / 2);
    });

    run_test("push after close is rejected", [] {
        BlockingQueue<int> queue;
        queue.close();
        CHECK(!queue.push(1));
        CHECK(!queue.pop().has_value());
    });

    run_test("close wakes a blocked consumer", [] {
        BlockingQueue<int> queue;
        std::atomic<bool> woke{false};
        std::thread consumer([&] {
            woke = !queue.pop().has_value();
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        queue.close();
        consumer.join();
        CHECK(woke);
    });
}
