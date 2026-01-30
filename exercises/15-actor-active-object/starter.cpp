#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

#include "common/blocking_queue.hpp"
#include "common/check.hpp"
#include "common/concurrent.hpp"

class Actor {
public:
    Actor() : worker_([this] { run(); }) {}

    ~Actor() {
        mailbox_.close();
    }

    Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

    void tell(std::function<void()> message) {
        // TODO: enqueue the message for the actor thread.
    }

    template <typename Function>
    auto ask(Function function) {
        using Result = std::invoke_result_t<Function>;
        // TODO: run `function` on the actor thread and deliver its result (or exception) via a future.
        return std::future<Result>{};
    }

private:
    void run() {
        // TODO: process messages one at a time until the mailbox is closed and drained.
    }

    BlockingQueue<std::function<void()>> mailbox_;
    std::jthread worker_;
};

class CounterActor {
public:
    void increment() {
        actor_.tell([this] { ++value_; });
    }

    int value() {
        return actor_.ask([this] { return value_; }).get();
    }

private:
    int value_ = 0;
    Actor actor_;
};

int main() {
    run_test("state touched only by the actor needs no mutex", [] {
        CounterActor counter;
        run_concurrently(8, [&](unsigned) {
            for (int i = 0; i < 10'000; ++i) {
                counter.increment();
            }
        });
        CHECK_EQ(counter.value(), 80'000);
    });

    run_test("ask propagates exceptions", [] {
        Actor actor;
        auto future = actor.ask([]() -> int { throw std::logic_error("bad request"); });
        bool caught = false;
        try {
            future.get();
        } catch (const std::logic_error&) {
            caught = true;
        }
        CHECK(caught);
    });

    run_test("messages are handled in order", [] {
        Actor actor;
        std::vector<int> seen;
        for (int i = 0; i < 100; ++i) {
            actor.tell([&seen, i] { seen.push_back(i); });
        }
        actor.ask([] {}).get();
        for (int i = 0; i < 100; ++i) {
            CHECK_EQ(seen[i], i);
        }
    });
}
