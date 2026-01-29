# Exercise 15: Actor / Active Object

**Level:** Advanced  
**Paradigm:** Actor model / message passing  
**Concepts:** mailbox, serialised access without locks, request/response with futures

## Problem

Implement `Actor`: an object with its own thread and mailbox.

- `tell(message)` enqueues a `std::function<void()>` and returns immediately.
- `ask(function)` runs `function` on the actor thread and returns a `std::future` of its result; exceptions must propagate.
- Messages are processed one at a time, in order. Destroying the actor processes what is queued and stops.

`CounterActor` then keeps a plain `int` with **no mutex**: only the actor thread ever touches it.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-15`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-15`).

## Hints

Try the exercise first; open these one at a time.

1. Reuse `BlockingQueue` from `common/blocking_queue.hpp` as the mailbox.
2. The actor thread is just `while (auto m = mailbox_.pop()) (*m)();`.
3. `ask` wraps `function` in a lambda that fulfils a `std::promise`; use `set_exception` in a catch block.
4. Handle `void` results with `if constexpr (std::is_void_v<Result>)`.
