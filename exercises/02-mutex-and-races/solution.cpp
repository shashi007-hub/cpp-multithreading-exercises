#include <atomic>
#include <mutex>

#include "common/check.hpp"
#include "common/concurrent.hpp"

class Account {
public:
    explicit Account(long long balance) : balance_(balance) {}

    void deposit(long long amount) {
        std::lock_guard lock(mutex_);
        balance_ += amount;
    }

    bool withdraw(long long amount) {
        std::lock_guard lock(mutex_);
        if (balance_ < amount) {
            return false;
        }
        balance_ -= amount;
        return true;
    }

    long long balance() const {
        std::lock_guard lock(mutex_);
        return balance_;
    }

private:
    mutable std::mutex mutex_;
    long long balance_;
};

int main() {
    run_test("concurrent deposits are not lost", [] {
        Account account(0);
        run_concurrently(8, [&](unsigned) {
            for (int i = 0; i < 200'000; ++i) {
                account.deposit(1);
            }
        });
        CHECK_EQ(account.balance(), 1'600'000);
    });

    run_test("balance never goes negative", [] {
        Account account(1000);
        std::atomic<int> successes{0};
        run_concurrently(8, [&](unsigned) {
            for (int i = 0; i < 1000; ++i) {
                if (account.withdraw(1)) {
                    ++successes;
                }
            }
        });
        CHECK_EQ(successes.load(), 1000);
        CHECK_EQ(account.balance(), 0);
    });
}
