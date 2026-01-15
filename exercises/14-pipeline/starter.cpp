#include <string>
#include <thread>
#include <vector>

#include "common/blocking_queue.hpp"
#include "common/check.hpp"

template <typename In, typename Out, typename Transform>
std::jthread make_stage(BlockingQueue<In>& input, BlockingQueue<Out>& output, Transform transform) {
    // TODO: pop from `input` until it is closed and drained, push transformed items to `output`, then close `output`.
    return std::jthread{};
}

std::vector<long long> run_pipeline(const std::vector<std::string>& lines) {
    BlockingQueue<std::string> raw;
    BlockingQueue<int> parsed;
    BlockingQueue<long long> squared;

    // TODO: connect raw -> parsed (std::stoi) -> squared (value * value) with two stages.

    for (const std::string& line : lines) {
        raw.push(line);
    }
    raw.close();

    std::vector<long long> results;
    while (auto value = squared.pop()) {
        results.push_back(*value);
    }
    return results;
}

int main() {
    run_test("stages transform items in order", [] {
        std::vector<std::string> lines;
        std::vector<long long> expected;
        for (int i = 1; i <= 20'000; ++i) {
            lines.push_back(std::to_string(i));
            expected.push_back(1LL * i * i);
        }
        CHECK(run_pipeline(lines) == expected);
    });

    run_test("empty input terminates", [] { CHECK(run_pipeline({}).empty()); });
}
