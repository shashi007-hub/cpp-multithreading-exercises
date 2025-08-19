# Exercise 12: Latches, Barriers and Semaphores

**Level:** Intermediate  
**Paradigm:** Coordination primitives (C++20)  
**Concepts:** `std::latch`, `std::barrier`, `std::counting_semaphore`

## Problem

Three coordination problems, three C++20 primitives.

1. `workers_released_together`: workers wait on a `std::latch`; the main thread opens it once and all of them proceed.
2. `neighbours_see_completed_phase`: workers write a value, synchronise on a `std::barrier`, then read their neighbour's value, repeatedly. Nobody may ever read a stale phase.
3. `max_concurrency`: a `std::counting_semaphore` limits how many tasks run at once; verify the observed peak never exceeds the limit.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-12`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-12`).
