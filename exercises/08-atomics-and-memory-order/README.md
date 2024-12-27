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

## Hints

Try the exercise first; open these one at a time.

1. Lock: acquire ordering on `test_and_set`. Unlock: release ordering on `clear`.
2. `compare_exchange_weak` refreshes `current` on failure, so the loop condition re-evaluates it automatically.
3. The payload is a non-atomic variable. Release/acquire on `ready` is what makes the write to it visible.
4. Use `std::this_thread::yield()` inside spin loops so you are kind to other threads.

## Solution walkthrough

Atomics give indivisible operations; **memory orders** give visibility guarantees.

- A lock needs *acquire* on lock and *release* on unlock. Together they make everything written in one critical section visible to the next, without paying for full sequential consistency.
- The CAS loop is the universal lock-free pattern: read, compute, try to swap, retry if someone else won. `weak` may fail spuriously, so it belongs in loops and is cheaper on ARM.
- In `Message`, the store to `payload` is sequenced before the release store; a reader that observes `ready == true` via an acquire load is guaranteed to see the payload. With `relaxed` operations this would be a data race.
- `relaxed` is enough for `update_max`: only the atomic variable itself matters, nothing else is published through it.

When in doubt use the default `seq_cst`. Weaker orders are an optimisation to apply after profiling and with ThreadSanitizer on.
