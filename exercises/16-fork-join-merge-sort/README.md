# Exercise 16: Fork-Join Parallel Merge Sort

**Level:** Intermediate  
**Paradigm:** Divide and conquer / fork-join  
**Concepts:** recursive parallelism, depth cutoff, granularity control

## Problem

Parallelise merge sort with fork-join.

Implement `parallel_merge_sort(begin, end, depth)`:

- **Fork**: run the left half asynchronously while the current thread sorts the right half.
- **Join**: wait for the left half, then `std::inplace_merge`.
- Stop forking when `depth` reaches zero or the range is small, and fall back to `std::sort`.

The test sorts two million integers and prints sequential versus parallel time.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-16`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-16`).

## Hints

Try the exercise first; open these one at a time.

1. Pass `begin`, `middle`, `depth - 1` to `std::async`; iterators are cheap to copy.
2. Do real work on the current thread instead of only waiting; otherwise you waste a thread.
3. Depth is about `log2(cores)` so that you create roughly one task per core.
4. Below a few thousand elements the thread overhead exceeds the sorting work.

## Solution walkthrough

Fork-join recursion naturally exposes parallelism: the two halves are independent.

- Unbounded forking would create a thread per tiny range and drown in overhead, so `depth` and the sequential cutoff control **granularity**. This tuning is the central skill in task parallelism.
- The calling thread sorts the right half itself while the async thread handles the left, so `depth` levels create `2^depth` leaf tasks with only `2^depth - 1` extra threads.
- The merge is sequential, so the top-level merge limits speedup (Amdahl's law). You should see a modest 2-4x speedup, not 8x.

Production libraries (TBB, OpenMP tasks, `std::execution::par`) use a work-stealing scheduler instead of one OS thread per fork.
