#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "common/check.hpp"
#include "common/concurrent.hpp"

struct Fork {
    std::mutex mutex;
    std::atomic<bool> in_use{false};
};

void eat(Fork& left, Fork& right, std::atomic<bool>& violation) {
    if (left.in_use.exchange(true) || right.in_use.exchange(true)) {
        violation = true;
    }
    std::this_thread::yield();
    left.in_use = false;
    right.in_use = false;
}

struct DinnerReport {
    std::vector<int> meals_eaten;
    bool forks_were_exclusive;
};

DinnerReport run_dinner(unsigned philosophers, int meals) {
    CHECK(philosophers >= 2);
    std::vector<Fork> forks(philosophers);
    std::vector<int> meals_eaten(philosophers, 0);
    std::atomic<bool> violation{false};

    run_concurrently(philosophers, [&](unsigned id) {
        Fork& left = forks[id];
        Fork& right = forks[(id + 1) % philosophers];
        for (int meal = 0; meal < meals; ++meal) {
            std::scoped_lock both_forks(left.mutex, right.mutex);
            eat(left, right, violation);
            ++meals_eaten[id];
        }
    });
    return {meals_eaten, !violation};
}

int main() {
    run_test("five philosophers finish without deadlock", [] {
        const DinnerReport report = run_dinner(5, 2000);
        CHECK(report.forks_were_exclusive);
        for (int eaten : report.meals_eaten) {
            CHECK_EQ(eaten, 2000);
        }
    });

    run_test("two philosophers share two forks", [] {
        const DinnerReport report = run_dinner(2, 5000);
        CHECK(report.forks_were_exclusive);
    });
}
