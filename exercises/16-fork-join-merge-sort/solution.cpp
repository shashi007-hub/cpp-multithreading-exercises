#include <algorithm>
#include <bit>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "common/check.hpp"
#include "common/stopwatch.hpp"

using Iterator = std::vector<int>::iterator;

constexpr std::ptrdiff_t kSequentialCutoff = 10'000;

void parallel_merge_sort(Iterator begin, Iterator end, unsigned depth) {
    const std::ptrdiff_t size = end - begin;
    if (size < 2) {
        return;
    }
    if (depth == 0 || size < kSequentialCutoff) {
        std::sort(begin, end);
        return;
    }
    const Iterator middle = begin + size / 2;
    std::future<void> left = std::async(std::launch::async, parallel_merge_sort, begin, middle, depth - 1);
    parallel_merge_sort(middle, end, depth - 1);
    left.get();
    std::inplace_merge(begin, middle, end);
}

void parallel_sort(std::vector<int>& values) {
    const unsigned depth = std::max(1u, static_cast<unsigned>(std::bit_width(std::thread::hardware_concurrency())));
    parallel_merge_sort(values.begin(), values.end(), depth);
}

std::vector<int> random_values(std::size_t count) {
    std::mt19937 engine(42);
    std::uniform_int_distribution<int> distribution(-1'000'000, 1'000'000);
    std::vector<int> values(count);
    std::generate(values.begin(), values.end(), [&] { return distribution(engine); });
    return values;
}

int main() {
    run_test("sorts random data like std::sort", [] {
        std::vector<int> values = random_values(2'000'000);
        std::vector<int> expected = values;

        Stopwatch watch;
        std::sort(expected.begin(), expected.end());
        const double sequential_ms = watch.elapsed_ms();

        watch.restart();
        parallel_sort(values);
        const double parallel_ms = watch.elapsed_ms();

        CHECK(values == expected);
        std::cout << "         sequential " << sequential_ms << " ms, parallel " << parallel_ms << " ms\n";
    });

    run_test("handles already sorted, reversed and duplicate inputs", [] {
        std::vector<int> sorted(100'000);
        std::iota(sorted.begin(), sorted.end(), 0);
        std::vector<int> reversed(sorted.rbegin(), sorted.rend());
        std::vector<int> duplicates(100'000, 7);
        parallel_sort(reversed);
        parallel_sort(duplicates);
        CHECK(reversed == sorted);
        CHECK(std::is_sorted(duplicates.begin(), duplicates.end()));
    });

    run_test("tiny inputs", [] {
        std::vector<int> empty;
        std::vector<int> one{5};
        parallel_sort(empty);
        parallel_sort(one);
        CHECK(empty.empty());
        CHECK_EQ(one[0], 5);
    });
}
