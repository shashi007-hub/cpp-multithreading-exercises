#include <algorithm>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "common/check.hpp"

using WordCounts = std::unordered_map<std::string, std::size_t>;

WordCounts count_words(const std::vector<std::string>& lines, std::size_t begin, std::size_t end) {
    WordCounts counts;
    for (std::size_t i = begin; i < end; ++i) {
        std::istringstream words(lines[i]);
        for (std::string word; words >> word;) {
            ++counts[word];
        }
    }
    return counts;
}

WordCounts parallel_word_count(const std::vector<std::string>& lines, unsigned workers) {
    // TODO map: give each worker a slice of lines and its own private WordCounts (no shared writes).
    // TODO reduce: merge the private maps on the calling thread after joining.
    return {};
}

std::vector<std::string> make_corpus(std::size_t lines) {
    const std::vector<std::string> vocabulary = {"lock", "thread", "atomic", "future", "queue", "barrier"};
    std::vector<std::string> corpus;
    for (std::size_t i = 0; i < lines; ++i) {
        std::string line;
        for (std::size_t j = 0; j < 8; ++j) {
            line += vocabulary[(i * 7 + j * 3) % vocabulary.size()] + ' ';
        }
        corpus.push_back(line);
    }
    return corpus;
}

int main() {
    run_test("matches the sequential result", [] {
        const auto corpus = make_corpus(20'000);
        const WordCounts expected = count_words(corpus, 0, corpus.size());
        for (unsigned workers : {1u, 2u, 5u, 8u}) {
            CHECK(parallel_word_count(corpus, workers) == expected);
        }
    });

    run_test("more workers than lines", [] {
        const auto corpus = make_corpus(3);
        CHECK(parallel_word_count(corpus, 8) == count_words(corpus, 0, corpus.size()));
    });
}
