# Exercise 06: Thread Pool

**Level:** Intermediate  
**Paradigm:** Task-based concurrency / executor  
**Concepts:** worker threads, `std::packaged_task`, `std::future`, clean destruction

## Problem

Implement a fixed-size `ThreadPool`.

- `submit(f, args...)` queues a task and returns a `std::future` of its result.
- Exceptions thrown by a task must be re-thrown by `future.get()`.
- Destroying the pool finishes every task already queued, then joins all workers.
- Submitting to a stopped pool throws.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-06`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-06`).

## Hints

Try the exercise first; open these one at a time.

1. `std::packaged_task` links a callable to a future. It is move-only, but `std::function` needs copyable callables, so hold it in a `std::shared_ptr`.
2. Workers loop: wait until `stopping_ || !tasks_.empty()`, exit only when the queue is empty.
3. Run the task **outside** the lock, otherwise the pool becomes single threaded.
4. Set `stopping_` under the lock, notify all, then join.

## Solution walkthrough

The pool decouples *what* to run from *which thread* runs it, and bounds the number of threads regardless of the number of tasks.

- `submit` wraps the call in a `packaged_task<Result()>`. Its future becomes the caller's handle; exceptions are captured inside the shared state, which is why `get()` re-throws them.
- Workers sleep on a condition variable and only exit when stopping *and* drained, which gives the "finish queued work" guarantee.
- Executing the task after releasing the mutex is essential: holding it would serialise all work.
- The destructor joins, so no worker can outlive the queue and mutex it uses (member order matters: threads are created last in practice).

Choosing the pool size: CPU-bound work wants about `hardware_concurrency()` threads; blocking I/O can use more.
