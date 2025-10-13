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

## Hints

Try the exercise first; open these one at a time.

1. A latch is one-shot (`count_down`, `wait`). A barrier is reusable across phases.
2. The barrier is constructed with the number of participants.
3. You need **two** barrier waits per iteration: one between write and read, one between read and the next write.
4. Always release the semaphore, ideally via a small RAII guard if the work can throw.

## Solution walkthrough

- **Latch**: a countdown that threads can wait on. Perfect for "start together" and "wait for N things to finish".
- **Barrier**: all participants block at `arrive_and_wait` until everyone has arrived, then the barrier resets for the next phase. This is the backbone of bulk-synchronous algorithms (simulations, iterative solvers).
- The second wait in the barrier loop is the subtle part: without it a fast thread could start overwriting its slot for phase *n+1* while a slow neighbour is still reading phase *n*.
- **Counting semaphore**: a permit pool. It limits concurrency (connection pools, rate limiting, bounded parallelism) rather than guarding data.

Note: ThreadSanitizer does not model `std::barrier` in libc++ and may report a false positive on exercise 12.

## Bonus challenges

- Use the barrier's completion function to swap two buffers between phases (double buffering).
- Implement a reusable `Barrier` yourself with a mutex and condition variable (generation counter).
- Write an RAII `Permit` type for the semaphore.
