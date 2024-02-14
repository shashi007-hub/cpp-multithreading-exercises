#pragma once

#include <cstdlib>
#include <iostream>

#define CHECK(condition)                                                            \
    do {                                                                            \
        if (!(condition)) {                                                         \
            std::cerr << __FILE__ << ':' << __LINE__ << " CHECK failed: " #condition \
                      << '\n';                                                      \
            std::exit(1);                                                           \
        }                                                                           \
    } while (false)

#define CHECK_EQ(actual, expected)                                                  \
    do {                                                                            \
        auto&& actual_value = (actual);                                             \
        auto&& expected_value = (expected);                                         \
        if (!(actual_value == expected_value)) {                                    \
            std::cerr << __FILE__ << ':' << __LINE__ << " CHECK_EQ failed: " #actual \
                      << " == " << actual_value << ", expected " << expected_value  \
                      << '\n';                                                      \
            std::exit(1);                                                           \
        }                                                                           \
    } while (false)

template <typename Test>
void run_test(const char* name, Test&& test) {
    test();
    std::cout << "[ pass ] " << name << '\n';
}
