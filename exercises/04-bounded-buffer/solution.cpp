#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#include "common/check.hpp"
#include "common/concurrent.hpp"

template <typename T>
class BoundedBuffer {
public:
    explicit BoundedBuffer(std::size_t capacity) : capacity_(capacity) {}

    void push(T value) {
        std::unique_lock lock(mutex_);
        not_full_.wait(lock, [this] { return items_.size() < capacity_; });
        items_.push_back(std::move(value));
        lock.unlock();
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock lock(mutex_);
        not_empty_.wait(lock, [this] { return !items_.empty(); });
        T value = std::move(items_.front());
        items_.pop_front();
        lock.unlock();
        not_full_.notify_one();
        return value;
    }

private:
    std::size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    std::deque<T> items_;
};

int main() {
    using namespace std::chrono_literals;

    run_test("many producers and consumers exchange every item", [] {
        constexpr unsigned kWorkers = 3;
        constexpr int kItemsPerProducer = 5000;
        BoundedBuffer<int> buffer(4);
        std::atomic<long long> consumed_sum{0};

        run_concurrently(2 * kWorkers, [&](unsigned id) {
            if (id < kWorkers) {
                for (int i = 1; i <= kItemsPerProducer; ++i) {
                    buffer.push(i);
                }
            } else {
                for (int i = 0; i < kItemsPerProducer; ++i) {
                    consumed_sum += buffer.pop();
                }
            }
        });

        const long long per_producer = static_cast<long long>(kItemsPerProducer) * (kItemsPerProducer + 1) / 2;
        CHECK_EQ(consumed_sum.load(), per_producer * kWorkers);
    });

    run_test("push blocks while the buffer is full", [] {
        BoundedBuffer<int> buffer(1);
        buffer.push(1);
        std::atomic<bool> second_push_done{false};
        std::thread producer([&] {
            buffer.push(2);
            second_push_done = true;
        });

        std::this_thread::sleep_for(100ms);
        CHECK(!second_push_done);
        CHECK_EQ(buffer.pop(), 1);
        producer.join();
        CHECK(second_push_done);
        CHECK_EQ(buffer.pop(), 2);
    });
}
