# Exercise 10: Reader-Writer Cache

**Level:** Intermediate  
**Paradigm:** Shared/exclusive locking  
**Concepts:** `std::shared_mutex`, `shared_lock`, double-checked lookup, compute-once caching

## Problem

Implement a thread-safe `Cache<Key, Value>` optimised for read-heavy workloads.

- `get` may run concurrently with other `get`s.
- `put` has exclusive access.
- `get_or_compute(key, fn)` returns the cached value, or computes it **exactly once** per key even when many threads miss simultaneously.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-10`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-10`).

## Hints

Try the exercise first; open these one at a time.

1. Readers take `std::shared_lock`, writers take `std::unique_lock` on the same `std::shared_mutex`.
2. You cannot upgrade a shared lock. Release it, take the unique lock, and look again.
3. The second lookup under the exclusive lock is what prevents duplicate computation.
4. Return values by copy (or `shared_ptr`): a reference into the map would dangle after a rehash.

## Solution walkthrough

Read-mostly data benefits from a lock that admits many readers at once.

- `get` uses a shared lock, so lookups scale with the number of cores as long as nobody writes.
- `get_or_compute` is *double-checked* under the proper locks: optimistic shared lookup, then exclusive lookup before computing. Two threads missing at once both reach the exclusive section, but only the first inserts; the second finds the value on its re-check.
- Computing under the exclusive lock blocks other keys during the computation. That is a deliberate simplification; the bonus lifts it.
- `shared_mutex` has higher overhead than `mutex`, so it only wins when reads dominate and critical sections are not tiny.
