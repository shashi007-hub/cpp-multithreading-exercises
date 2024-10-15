# Exercise 07: Futures, Promises and async

**Level:** Intermediate  
**Paradigm:** Asynchronous results / dataflow  
**Concepts:** `std::async`, `std::promise`, `std::future`, `std::shared_future`, `wait_for`

## Problem

Implement five small tasks with the futures toolbox.

1. `count_primes_async(limit, parts)` splits `[0, limit)` into parts and counts primes with `std::async(std::launch::async, ...)`.
2. `hand_off_value(v)` computes `v * 2` on another thread and delivers it through a `std::promise`.
3. `failure_reaches_caller()` fails a promise with an exception and shows that `get()` re-throws it.
4. `finishes_within(budget, work)` reports whether an async sleep finished before a deadline.
5. `broadcast_sum(readers)` lets several threads wait on one `std::shared_future`.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-07`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-07`).
