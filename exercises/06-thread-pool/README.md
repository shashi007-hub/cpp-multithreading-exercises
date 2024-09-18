# Exercise 06: Thread Pool

**Level:** Intermediate  
**Paradigm:** Task-based concurrency / executor  
**Concepts:** worker threads, `std::packaged_task`, `std::future`, clean destruction

## Problem

Implement a fixed-size `ThreadPool`.

- `submit(f, args...)` queues a task and returns a `std::future` of its result.
- Exceptions thrown by a task must be re-thrown by `future.get()`.
- Destroying the pool finishes every task already queued, then joins all workers.
- Submitting to a stopped pool throws.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-06`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-06`).
