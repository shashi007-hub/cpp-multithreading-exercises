#include <algorithm>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <vector>

#include "common/check.hpp"
#include "common/concurrent.hpp"
#include "common/stopwatch.hpp"

constexpr std::size_t kCacheLine = 64;

struct PackedSlot {
    std::atomic<long long> value{0};
};

struct PaddedSlot {
    // TODO: give each slot its own cache line so neighbouring threads stop invalidating each other.
    std::atomic<long long> value{0};
};

std::size_t this_thread_index() {
    static std::atomic<std::size_t> next_index{0};
    // TODO: hand every thread a stable, unique index the first time it asks (thread_local).
    return 0;
}

template <typename Slot>
class ShardedCounter {
public:
    explicit ShardedCounter(std::size_t shards) : slots_(shards) {}

    void add(long long delta) {
        slots_[this_thread_index() % slots_.size()].value.fetch_add(delta, std::memory_order_relaxed);
    }

    long long total() const {
        long long sum = 0;
        for (const Slot& slot : slots_) {
            sum += slot.value.load(std::memory_order_relaxed);
        }
        return sum;
    }

private:
    std::vector<Slot> slots_;
};

template <typename Slot>
double measure_ms(unsigned threads, int increments) {
    ShardedCounter<Slot> counter(threads);
    Stopwatch watch;
    run_concurrently(threads, [&](unsigned) {
        for (int i = 0; i < increments; ++i) {
            counter.add(1);
        }
    });
    const double elapsed = watch.elapsed_ms();
    CHECK_EQ(counter.total(), static_cast<long long>(threads) * increments);
    return elapsed;
}

int main() {
    run_test("padded slots occupy a full cache line", [] {
        CHECK(sizeof(PaddedSlot) >= kCacheLine);
        CHECK(alignof(PaddedSlot) >= kCacheLine);
    });

    run_test("each thread gets a unique stable index", [] {
        std::vector<std::size_t> first(8);
        std::vector<std::size_t> second(8);
        run_concurrently(8, [&](unsigned id) {
            first[id] = this_thread_index();
            second[id] = this_thread_index();
        });
        CHECK(first == second);
        std::sort(first.begin(), first.end());
        CHECK(std::adjacent_find(first.begin(), first.end()) == first.end());
    });

    run_test("sharded counters add up", [] {
        const double packed = measure_ms<PackedSlot>(4, 3'000'000);
        const double padded = measure_ms<PaddedSlot>(4, 3'000'000);
        std::cout << "         packed " << packed << " ms, padded " << padded << " ms\n";
    });
}
