#pragma once

#include <chrono>

class Stopwatch {
public:
    using Clock = std::chrono::steady_clock;

    void restart() { start_ = Clock::now(); }

    double elapsed_ms() const {
        return std::chrono::duration<double, std::milli>(Clock::now() - start_).count();
    }

private:
    Clock::time_point start_ = Clock::now();
};
