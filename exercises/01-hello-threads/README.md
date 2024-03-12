# Exercise 01: Hello, Threads

**Level:** Beginner  
**Paradigm:** Thread-per-task / data parallelism  
**Concepts:** `std::thread`, `join`, capturing by reference, `std::ref`, partitioning work

## Problem

Split a big array across worker threads and add it up.

1. Implement `parallel_sum(data, workers)`: each worker sums one contiguous chunk into its own slot of `partial_sums`; the caller joins every thread and adds the slots.
2. Implement `squares_in_thread(count)`: start a `std::thread` that fills a `std::vector<int>` it receives **by reference**, and return that vector.

The function must work for an empty input, for more workers than elements, and for any worker count.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-01`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-01`).
