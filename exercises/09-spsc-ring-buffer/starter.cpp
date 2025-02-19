#include <array>
#include <atomic>
#include <cstddef>
#include <optional>
#include <thread>

#include "common/check.hpp"

template <typename T, std::size_t Capacity>
class SpscRingBuffer {
    static_assert((Capacity & (Capacity - 1)) == 0, "capacity must be a power of two");

public:
    bool try_push(T value) {
        // TODO: producer side. Fail when full; write the slot, then publish tail_ with release.
        return false;
    }

    std::optional<T> try_pop() {
        // TODO: consumer side. Fail when empty; read the slot, then publish head_ with release.
        return std::nullopt;
    }

private:
    static constexpr std::size_t kMask = Capacity - 1;

    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::atomic<std::size_t> tail_{0};
    std::array<T, Capacity> slots_{};
};

int main() {
    run_test("single thread fill and drain", [] {
        SpscRingBuffer<int, 4> ring;
        for (int i = 0; i < 4; ++i) {
            CHECK(ring.try_push(i));
        }
        CHECK(!ring.try_push(99));
        for (int i = 0; i < 4; ++i) {
            CHECK_EQ(*ring.try_pop(), i);
        }
        CHECK(!ring.try_pop().has_value());
    });

    run_test("producer and consumer preserve order", [] {
        constexpr int kItems = 1'000'000;
        SpscRingBuffer<int, 1024> ring;
        bool in_order = true;
        std::thread consumer([&] {
            for (int expected = 0; expected < kItems;) {
                if (auto item = ring.try_pop()) {
                    in_order &= (*item == expected);
                    ++expected;
                }
            }
        });
        for (int i = 0; i < kItems;) {
            if (ring.try_push(i)) {
                ++i;
            }
        }
        consumer.join();
        CHECK(in_order);
    });
}
