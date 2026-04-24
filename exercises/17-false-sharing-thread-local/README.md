# Exercise 17: False Sharing and thread_local

**Level:** Advanced  
**Paradigm:** Performance / data layout  
**Concepts:** cache lines, `alignas`, `thread_local`, sharded counters

## Problem

Make a sharded counter fast.

1. Give `PaddedSlot` its own cache line using `alignas(kCacheLine)`.
2. Implement `this_thread_index()`: every thread receives a stable, unique index the first time it asks, using `thread_local`.

The benchmark increments counters from four threads using packed versus padded slots and prints both timings. Both must produce the right total, but one should be dramatically faster.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-17`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-17`).
