# Exercise 11: Striped Concurrent Hash Map

**Level:** Intermediate  
**Paradigm:** Fine-grained locking  
**Concepts:** lock striping, contention, per-bucket invariants

## Problem

One global lock around a hash map serialises every operation. Implement `StripedMap<Key, Value, Stripes>` where keys are distributed over independent stripes, each with its own mutex.

Provide `insert_or_assign`, `find`, `erase`, an atomic `update(key, fn)` and `size()`. Operations on keys in different stripes must not block each other.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-11`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-11`).
