#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <unordered_set>
#include <vector>

#include "common/check.hpp"
#include "common/concurrent.hpp"

class TimerScheduler {
public:
    using Clock = std::chrono::steady_clock;
    using TaskId = std::uint64_t;

    TimerScheduler() : worker_([this](std::stop_token stop) { run(stop); }) {}

    ~TimerScheduler() { worker_.request_stop(); }

    TimerScheduler(const TimerScheduler&) = delete;
    TimerScheduler& operator=(const TimerScheduler&) = delete;

    TaskId schedule_after(std::chrono::milliseconds delay, std::function<void()> task) {
        std::lock_guard lock(mutex_);
        const TaskId id = next_id_++;
        queue_.push(Entry{Clock::now() + delay, id, std::move(task)});
        pending_.insert(id);
        wake_.notify_one();
        return id;
    }

    bool cancel(TaskId id) {
        std::lock_guard lock(mutex_);
        return pending_.erase(id) > 0;
    }

private:
    struct Entry {
        Clock::time_point due;
        TaskId id;
        std::function<void()> task;

        bool operator>(const Entry& other) const { return due > other.due; }
    };

    void run(std::stop_token stop) {
        std::unique_lock lock(mutex_);
        while (!stop.stop_requested()) {
            if (queue_.empty()) {
                wake_.wait(lock, stop, [this] { return !queue_.empty(); });
                continue;
            }
            const Clock::time_point due = queue_.top().due;
            wake_.wait_until(lock, stop, due, [&] { return queue_.top().due < due; });
            if (stop.stop_requested() || queue_.top().due > Clock::now()) {
                continue;
            }
            Entry entry = queue_.top();
            queue_.pop();
            if (pending_.erase(entry.id) == 0) {
                continue;
            }
            lock.unlock();
            entry.task();
            lock.lock();
        }
    }

    std::mutex mutex_;
    std::condition_variable_any wake_;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<>> queue_;
    std::unordered_set<TaskId> pending_;
    TaskId next_id_ = 1;
    std::jthread worker_;
};

int main() {
    using namespace std::chrono_literals;

    run_test("tasks run in deadline order", [] {
        std::mutex mutex;
        std::vector<int> order;
        auto record = [&](int label) {
            return [&, label] {
                std::lock_guard lock(mutex);
                order.push_back(label);
            };
        };
        {
            TimerScheduler scheduler;
            scheduler.schedule_after(120ms, record(1));
            scheduler.schedule_after(40ms, record(2));
            scheduler.schedule_after(80ms, record(3));
            std::this_thread::sleep_for(250ms);
        }
        CHECK(order == (std::vector<int>{2, 3, 1}));
    });

    run_test("a later but earlier-due task wakes the worker", [] {
        std::atomic<bool> ran{false};
        TimerScheduler scheduler;
        scheduler.schedule_after(10s, [] {});
        std::this_thread::sleep_for(30ms);
        scheduler.schedule_after(20ms, [&] { ran = true; });
        std::this_thread::sleep_for(150ms);
        CHECK(ran);
    });

    run_test("cancelled tasks never run", [] {
        std::atomic<int> ran{0};
        TimerScheduler scheduler;
        const auto keep = scheduler.schedule_after(50ms, [&] { ++ran; });
        const auto drop = scheduler.schedule_after(50ms, [&] { ran += 100; });
        CHECK(scheduler.cancel(drop));
        CHECK(!scheduler.cancel(drop));
        std::this_thread::sleep_for(200ms);
        CHECK(!scheduler.cancel(keep));
        CHECK_EQ(ran.load(), 1);
    });

    run_test("many threads can schedule concurrently", [] {
        std::atomic<int> ran{0};
        {
            TimerScheduler scheduler;
            run_concurrently(8, [&](unsigned) {
                for (int i = 0; i < 100; ++i) {
                    scheduler.schedule_after(std::chrono::milliseconds(i % 20), [&] { ++ran; });
                }
            });
            std::this_thread::sleep_for(300ms);
        }
        CHECK_EQ(ran.load(), 800);
    });

    run_test("destruction discards pending tasks quickly", [] {
        const auto start = std::chrono::steady_clock::now();
        {
            TimerScheduler scheduler;
            scheduler.schedule_after(1h, [] {});
        }
        CHECK(std::chrono::steady_clock::now() - start < 2s);
    });
}
