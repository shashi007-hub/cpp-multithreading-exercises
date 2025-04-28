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
