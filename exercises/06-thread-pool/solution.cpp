#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

#include "common/check.hpp"

class ThreadPool {
public:
    explicit ThreadPool(unsigned workers) {
        for (unsigned i = 0; i < workers; ++i) {
            workers_.emplace_back([this] { work(); });
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool() {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        wake_.notify_all();
        for (std::thread& worker : workers_) {
            worker.join();
        }
    }

    template <typename Function, typename... Args>
    auto submit(Function&& function, Args&&... args) {
        using Result = std::invoke_result_t<Function, Args...>;
        auto task = std::make_shared<std::packaged_task<Result()>>(
            std::bind(std::forward<Function>(function), std::forward<Args>(args)...));
        std::future<Result> future = task->get_future();
        {
            std::lock_guard lock(mutex_);
            if (stopping_) {
                throw std::runtime_error("submit on a stopped pool");
            }
            tasks_.push([task] { (*task)(); });
        }
        wake_.notify_one();
        return future;
    }

private:
    void work() {
        for (;;) {
            std::function<void()> task;
            {
                std::unique_lock lock(mutex_);
                wake_.wait(lock, [this] { return stopping_ || !tasks_.empty(); });
                if (tasks_.empty()) {
                    return;
                }
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            task();
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable wake_;
    bool stopping_ = false;
};

int main() {
    run_test("results come back through futures", [] {
        ThreadPool pool(4);
        std::vector<std::future<long long>> futures;
        for (long long i = 0; i < 1000; ++i) {
            futures.push_back(pool.submit([](long long x) { return x * x; }, i));
        }
        long long sum = 0;
        for (auto& future : futures) {
            sum += future.get();
        }
        CHECK_EQ(sum, 332'833'500LL);
    });

    run_test("exceptions travel through the future", [] {
        ThreadPool pool(2);
        auto future = pool.submit([]() -> int { throw std::runtime_error("boom"); });
        bool caught = false;
        try {
            future.get();
        } catch (const std::runtime_error&) {
            caught = true;
        }
        CHECK(caught);
    });

    run_test("destructor finishes queued work", [] {
        std::atomic<int> finished{0};
        {
            ThreadPool pool(2);
            for (int i = 0; i < 100; ++i) {
                pool.submit([&] {
                    std::this_thread::sleep_for(std::chrono::microseconds(200));
                    ++finished;
                });
            }
        }
        CHECK_EQ(finished.load(), 100);
    });
}
