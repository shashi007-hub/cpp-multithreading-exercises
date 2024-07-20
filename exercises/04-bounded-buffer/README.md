# Exercise 04: Bounded Buffer (Producer/Consumer)

**Level:** Beginner  
**Paradigm:** Monitor / condition variables  
**Concepts:** `std::condition_variable`, predicates, spurious wake-ups, back-pressure

## Problem

Implement `BoundedBuffer<T>`, a fixed-capacity FIFO shared by producers and consumers.

- `push` blocks while the buffer is full.
- `pop` blocks while the buffer is empty.
- Multiple producers and consumers may use it simultaneously.

The tests check that nothing is lost or duplicated and that a producer really blocks when the buffer is full.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-04`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-04`).
