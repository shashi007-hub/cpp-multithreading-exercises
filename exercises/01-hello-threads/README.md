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

## Hints

Try the exercise first; open these one at a time.

1. Each thread writes to a different element of `partial_sums`, so no lock is needed.
2. The last chunk is usually shorter: clamp `begin` and `end` with `std::min(data.size(), ...)`.
3. `std::thread` copies its arguments. Wrap the vector in `std::ref(result)` to pass a reference.
4. Forgetting `join()` calls `std::terminate` in the thread destructor.

## Solution walkthrough

`parallel_sum` is the simplest shape of data parallelism: **partition, compute independently, combine**.

- The chunk size is `ceil(n / workers)`. Clamping both ends keeps trailing workers from reading past the array and turns "more workers than items" into harmless empty ranges.
- Every worker owns one `partial_sums[worker]`, so there is no shared write and therefore no race. The final reduction happens on the calling thread after all joins; `join()` also provides the happens-before edge that makes the workers' writes visible.
- `std::thread` decays and copies its arguments, which is why a reference parameter needs `std::ref`. Without it the code does not compile (or worse, mutates a copy in other APIs).

Adjacent slots share cache lines, which costs performance but not correctness; exercise 17 returns to that.

## Bonus challenges

- Replace the manual chunking with `std::thread::hardware_concurrency()` workers by default and measure where adding threads stops helping.
- Make `parallel_sum` a template over any arithmetic type and any random-access range.
- Try summing 100 elements with 8 threads. Is it faster than sequential? Explain why or why not.
