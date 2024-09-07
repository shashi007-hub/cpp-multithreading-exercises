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

## Hints

Try the exercise first; open these one at a time.

1. The wait predicate is `closed_ || !items_.empty()`.
2. After waking, check `items_.empty()` first: if so, the queue was closed and drained.
3. `close()` must use `notify_all`, since any number of consumers may be waiting.
4. Set `closed_` while holding the mutex, otherwise a consumer can miss the notification and sleep forever.

## Solution walkthrough

Closing a channel is the cleanest way to say "no more work". It replaces poison-pill sentinels and works with any number of consumers.

- The state change (`closed_ = true`) happens under the mutex. If it did not, a consumer could evaluate the predicate (false), then `close()` could set the flag and notify, and only then would the consumer begin waiting: a **lost wake-up**.
- `pop` drains remaining items after close, so closing never drops data; this matches Go channels and Rust's `mpsc`.
- `std::optional<T>` is the return type that cleanly encodes "no item".

This class is intentionally unbounded; exercise 4 shows how bounding adds back-pressure.

## Bonus challenges

- Add `try_pop()` and `pop_for(duration)`.
- Add a `size()` accessor. Why is its result immediately stale?
- Make a bounded variant that combines this exercise with exercise 4.
