#include <algorithm>
#include <functional>
#include <numeric>
#include <thread>
#include <vector>

#include "common/check.hpp"

long long parallel_sum(const std::vector<int>& data, unsigned workers) {
    std::vector<long long> partial_sums(workers, 0);
    std::vector<std::thread> threads;
    const std::size_t chunk = (data.size() + workers - 1) / workers;

    for (unsigned worker = 0; worker < workers; ++worker) {
        threads.emplace_back([&, worker] {
            const std::size_t begin = std::min(data.size(), worker * chunk);
            const std::size_t end = std::min(data.size(), begin + chunk);
            partial_sums[worker] = std::accumulate(data.begin() + begin, data.begin() + end, 0LL);
        });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    return std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
}

void fill_squares(std::vector<int>& out, int count) {
    for (int i = 0; i < count; ++i) {
        out.push_back(i * i);
    }
}

std::vector<int> squares_in_thread(int count) {
    std::vector<int> result;
    std::thread worker(fill_squares, std::ref(result), count);
    worker.join();
    return result;
}

int main() {
    run_test("parallel_sum matches closed form", [] {
        std::vector<int> data(1'000'000);
        std::iota(data.begin(), data.end(), 1);
        const long long expected = 1'000'000LL * 1'000'001 / 2;
        for (unsigned workers : {1u, 3u, 8u}) {
            CHECK_EQ(parallel_sum(data, workers), expected);
        }
    });

    run_test("parallel_sum handles empty input", [] { CHECK_EQ(parallel_sum({}, 4), 0); });

    run_test("squares_in_thread passes by reference", [] {
        const std::vector<int> squares = squares_in_thread(5);
        CHECK(squares == (std::vector<int>{0, 1, 4, 9, 16}));
    });
}
