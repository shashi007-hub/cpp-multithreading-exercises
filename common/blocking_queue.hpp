#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

template <typename T>
class BlockingQueue {
public:
    bool push(T value) {
        {
            std::lock_guard lock(mutex_);
            if (closed_) {
                return false;
            }
            items_.push_back(std::move(value));
        }
        ready_.notify_one();
        return true;
    }

    std::optional<T> pop() {
        std::unique_lock lock(mutex_);
        ready_.wait(lock, [this] { return closed_ || !items_.empty(); });
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        return value;
    }

    void close() {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        ready_.notify_all();
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::deque<T> items_;
    bool closed_ = false;
};
