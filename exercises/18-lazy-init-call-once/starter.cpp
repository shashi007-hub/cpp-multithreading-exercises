#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <optional>
#include <set>
#include <thread>

#include "common/check.hpp"
#include "common/concurrent.hpp"

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> factory) : factory_(std::move(factory)) {}

    const T& get() {
        // TODO: build the value exactly once, even when many threads race here (std::call_once).
        static T placeholder{};
        return placeholder;
    }

private:
    std::function<T()> factory_;
    std::once_flag once_;
    std::optional<T> value_;
};

template <typename T>
class DoubleCheckedSingleton {
public:
    explicit DoubleCheckedSingleton(std::function<T()> factory) : factory_(std::move(factory)) {}

    ~DoubleCheckedSingleton() { delete instance_.load(); }

    DoubleCheckedSingleton(const DoubleCheckedSingleton&) = delete;
    DoubleCheckedSingleton& operator=(const DoubleCheckedSingleton&) = delete;

    T& get() {
        // TODO: fast path with an acquire load, slow path under mutex_, publish with a release store.
        static T placeholder{};
        return placeholder;
    }

private:
    std::function<T()> factory_;
    std::atomic<T*> instance_{nullptr};
    std::mutex mutex_;
};

struct Resource {
    int id = 0;
};

std::atomic<int> g_constructions{0};

Resource make_slow_resource() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return Resource{++g_constructions};
}

Resource& meyers_singleton() {
    // TODO: a function-local static is initialised thread-safely since C++11.
    static Resource placeholder;
    return placeholder;
}

template <typename Accessor>
std::size_t distinct_addresses(Accessor accessor) {
    std::set<const void*> addresses;
    std::mutex mutex;
    run_concurrently(16, [&](unsigned) {
        const void* address = &accessor();
        std::lock_guard lock(mutex);
        addresses.insert(address);
    });
    return addresses.size();
}

int main() {
    run_test("LazyValue builds once", [] {
        std::atomic<int> builds{0};
        LazyValue<int> lazy([&] {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            return ++builds;
        });
        CHECK_EQ(distinct_addresses([&]() -> const int& { return lazy.get(); }), 1);
        CHECK_EQ(builds.load(), 1);
        CHECK_EQ(lazy.get(), 1);
    });

    run_test("double-checked singleton builds once", [] {
        std::atomic<int> builds{0};
        DoubleCheckedSingleton<Resource> singleton([&] {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            return Resource{++builds};
        });
        CHECK_EQ(distinct_addresses([&]() -> Resource& { return singleton.get(); }), 1);
        CHECK_EQ(builds.load(), 1);
    });

    run_test("function-local static builds once", [] {
        g_constructions = 0;
        CHECK_EQ(distinct_addresses([]() -> Resource& { return meyers_singleton(); }), 1);
        CHECK_EQ(g_constructions.load(), 1);
    });
}
