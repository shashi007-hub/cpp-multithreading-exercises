# Exercise 08: Atomics and Memory Ordering

**Level:** Advanced  
**Paradigm:** Lock-free primitives  
**Concepts:** `std::atomic`, compare-exchange, acquire/release, spin locks

## Problem

Build three lock-free building blocks.

1. `SpinLock` on top of `std::atomic_flag` that works with `std::lock_guard`.
2. `update_max(std::atomic<int>&, candidate)` that raises an atomic to the maximum of all candidates using a compare-exchange loop.
3. `Message::publish` / `wait_and_read`: hand a plain `int` payload from one thread to another using a release store and an acquire load on a `ready` flag.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-08`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-08`).
