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
        // TODO: tell workers to stop once the queue is drained, then join them.
    }

    template <typename Function, typename... Args>
    auto submit(Function&& function, Args&&... args) {
        using Result = std::invoke_result_t<Function, Args...>;
        // TODO: wrap the call in a packaged_task, queue it, and return its future.
        return std::future<Result>{};
    }

private:
    void work() {
        // TODO: loop taking tasks until the pool is stopping and the queue is empty.
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
