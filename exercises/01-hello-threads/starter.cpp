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

    // TODO: start one thread per worker, each summing its own chunk into partial_sums[worker].
    // TODO: join every thread before reading partial_sums.
    return std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
}

std::vector<int> squares_in_thread(int count) {
    // TODO: run a thread that appends i * i for i in [0, count) to a vector it receives by reference.
    return {};
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
