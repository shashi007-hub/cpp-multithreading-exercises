# Exercise 05: Blocking Queue with Close

**Level:** Intermediate  
**Paradigm:** Message passing / graceful shutdown  
**Concepts:** unbounded queue, close semantics, `std::optional`, shutdown without sentinels

## Problem

Build the queue every later exercise relies on: `BlockingQueue<T>`.

- `push` enqueues and returns `true`, or returns `false` once the queue is closed.
- `pop` blocks until an item exists. It returns `std::nullopt` only when the queue is **closed and empty**.
- `close` wakes every blocked consumer. Items already queued must still be delivered.

The reference version of your finished queue is also provided as `common/blocking_queue.hpp` for exercises 14 and 15.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-05`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-05`).
