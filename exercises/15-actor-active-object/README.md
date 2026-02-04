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

## Solution walkthrough

Instead of protecting shared state with locks, **confine** it to one thread and send that thread messages. This is the actor model (Erlang, Akka) and the *Active Object* pattern.

- Because one thread processes messages sequentially, the state needs no synchronisation, and handlers are free of data races by construction.
- `tell` is fire-and-forget; `ask` is request/response via future.
- The mailbox is the only shared object, and it is already thread safe.
- Cost: every operation pays a queue hop and a context switch, so actors suit coarse-grained stateful components (sessions, devices, caches), not tight loops.

Destruction order matters: the destructor closes the mailbox first, then the `jthread` member joins after the queue drains.

## Bonus challenges

- Add typed messages with `std::variant` and `std::visit` instead of `std::function`.
- Let actors reference each other and build a ping-pong benchmark.
- Add supervision: restart an actor when a message handler throws.
