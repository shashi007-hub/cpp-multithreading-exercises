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
        {
            std::lock_guard lock(mutex_);
            if (closed_) {
                return false;
            }
            items_.push_back(std::move(value));
        }
        ready_.notify_one();
        return true;
    }

    std::optional<T> pop() {
        std::unique_lock lock(mutex_);
        ready_.wait(lock, [this] { return closed_ || !items_.empty(); });
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        return value;
    }

    void close() {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        ready_.notify_all();
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
