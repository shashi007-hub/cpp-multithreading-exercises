# Exercise 09: Lock-Free SPSC Ring Buffer

**Level:** Advanced  
**Paradigm:** Lock-free data structures  
**Concepts:** single-producer/single-consumer queue, power-of-two masks, cache-line alignment

## Problem

Implement `try_push` and `try_pop` of a fixed-capacity single-producer/single-consumer ring buffer without any lock.

- Exactly one thread calls `try_push`, exactly one calls `try_pop`.
- Neither call may block; they report full/empty instead.
- Items must arrive in order. The test pushes a million integers.

`head_` (consumer-owned) and `tail_` (producer-owned) are monotonically increasing counters; slots are addressed with `index & (Capacity - 1)`.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-09`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-09`).
