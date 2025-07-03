#include <array>
#include <functional>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "common/check.hpp"
#include "common/concurrent.hpp"

template <typename Key, typename Value, std::size_t Stripes = 16>
class StripedMap {
public:
    void insert_or_assign(const Key& key, Value value) {
        Stripe& stripe = stripe_for(key);
        std::lock_guard lock(stripe.mutex);
        stripe.entries.insert_or_assign(key, std::move(value));
    }

    std::optional<Value> find(const Key& key) const {
        const Stripe& stripe = stripe_for(key);
        std::lock_guard lock(stripe.mutex);
        auto it = stripe.entries.find(key);
        if (it == stripe.entries.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    bool erase(const Key& key) {
        Stripe& stripe = stripe_for(key);
        std::lock_guard lock(stripe.mutex);
        return stripe.entries.erase(key) > 0;
    }

    void update(const Key& key, const std::function<void(Value&)>& modify) {
        Stripe& stripe = stripe_for(key);
        std::lock_guard lock(stripe.mutex);
        modify(stripe.entries[key]);
    }

    std::size_t size() const {
        std::size_t total = 0;
        for (const Stripe& stripe : stripes_) {
            std::lock_guard lock(stripe.mutex);
            total += stripe.entries.size();
        }
        return total;
    }

private:
    struct Stripe {
        mutable std::mutex mutex;
        std::unordered_map<Key, Value> entries;
    };

    Stripe& stripe_for(const Key& key) { return stripes_[std::hash<Key>{}(key) % Stripes]; }
    const Stripe& stripe_for(const Key& key) const { return stripes_[std::hash<Key>{}(key) % Stripes]; }

    std::array<Stripe, Stripes> stripes_;
};

int main() {
    run_test("basic operations", [] {
        StripedMap<int, int> map;
        map.insert_or_assign(1, 10);
        map.insert_or_assign(1, 11);
        CHECK_EQ(*map.find(1), 11);
        CHECK(map.erase(1));
        CHECK(!map.erase(1));
        CHECK_EQ(map.size(), 0);
    });

    run_test("concurrent updates are atomic per key", [] {
        StripedMap<int, long long> map;
        run_concurrently(8, [&](unsigned) {
            for (int i = 0; i < 50'000; ++i) {
                map.update(i % 100, [](long long& value) { ++value; });
            }
        });
        CHECK_EQ(map.size(), 100);
        long long total = 0;
        for (int key = 0; key < 100; ++key) {
            total += *map.find(key);
        }
        CHECK_EQ(total, 400'000);
    });

    run_test("concurrent inserts of distinct keys", [] {
        StripedMap<int, int> map;
        run_concurrently(4, [&](unsigned id) {
            for (int i = 0; i < 5000; ++i) {
                map.insert_or_assign(static_cast<int>(id) * 5000 + i, i);
            }
        });
        CHECK_EQ(map.size(), 20'000);
    });
}
