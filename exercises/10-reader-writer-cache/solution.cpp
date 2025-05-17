#include <atomic>
#include <functional>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "common/check.hpp"
#include "common/concurrent.hpp"

template <typename Key, typename Value>
class Cache {
public:
    std::optional<Value> get(const Key& key) const {
        std::shared_lock lock(mutex_);
        return find(key);
    }

    void put(const Key& key, Value value) {
        std::unique_lock lock(mutex_);
        entries_[key] = std::move(value);
    }

    Value get_or_compute(const Key& key, const std::function<Value(const Key&)>& compute) {
        {
            std::shared_lock lock(mutex_);
            if (auto cached = find(key)) {
                return *cached;
            }
        }
        std::unique_lock lock(mutex_);
        if (auto cached = find(key)) {
            return *cached;
        }
        return entries_.emplace(key, compute(key)).first->second;
    }

private:
    std::optional<Value> find(const Key& key) const {
        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    mutable std::shared_mutex mutex_;
    std::unordered_map<Key, Value> entries_;
};

int main() {
    run_test("put then get", [] {
        Cache<std::string, int> cache;
        CHECK(!cache.get("a").has_value());
        cache.put("a", 1);
        CHECK_EQ(*cache.get("a"), 1);
    });

    run_test("mixed readers and writers stay consistent", [] {
        Cache<int, int> cache;
        run_concurrently(8, [&](unsigned id) {
            for (int i = 0; i < 20'000; ++i) {
                const int key = i % 64;
                if (id == 0) {
                    cache.put(key, key * 2);
                } else if (auto value = cache.get(key)) {
                    CHECK_EQ(*value, key * 2);
                }
            }
        });
    });

    run_test("get_or_compute computes each key once", [] {
        Cache<int, int> cache;
        std::atomic<int> computations{0};
        run_concurrently(8, [&](unsigned) {
            for (int key = 0; key < 100; ++key) {
                const int value = cache.get_or_compute(key, [&](const int& k) {
                    ++computations;
                    return k + 1000;
                });
                CHECK_EQ(value, key + 1000);
            }
        });
        CHECK_EQ(computations.load(), 100);
    });
}
