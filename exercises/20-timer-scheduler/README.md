# Exercise 20: Capstone: Timer Scheduler

**Level:** Advanced  
**Paradigm:** Event loop / timed executor  
**Concepts:** priority queue, `wait_until`, wake-ups, cancellation, clean shutdown

## Problem

Build a `TimerScheduler` like the one inside every GUI toolkit, game engine and RPC framework.

- `schedule_after(delay, task)` returns a task id; tasks run on a single worker thread in deadline order.
- `cancel(id)` returns `true` only if the task had not run or been cancelled yet.
- Scheduling a task with an earlier deadline than the current wait must wake the worker.
- Many threads may schedule concurrently.
- Destroying the scheduler discards pending tasks promptly, even if one is due in an hour.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-20`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-20`).

## Hints

Try the exercise first; open these one at a time.

1. A min-heap (`priority_queue` with `std::greater`) keeps the next deadline on top.
2. Track live ids in an `unordered_set`; cancelling erases from it, and the worker skips entries that are no longer pending.
3. `wait_until(lock, stop, deadline, predicate)` with predicate "an earlier task arrived" handles re-planning.
4. Copy the entry out of the queue and run it with the lock released so tasks can schedule more tasks.

## Solution walkthrough

This capstone combines most earlier techniques: a mutex-guarded priority queue, a condition variable with timed waits, `jthread` plus `stop_token` for shutdown, and futures-free fire-and-forget execution.

- The worker always sleeps until the **earliest** deadline. A new task with a sooner deadline changes the plan, which is why `schedule_after` notifies and the wait predicate checks `top().due < due`.
- Cancellation is lazy: removing from the middle of a heap is awkward, so ids are removed from `pending_` and stale heap entries are skipped when they surface.
- Tasks run **without** the lock held. Otherwise a task calling `schedule_after` would deadlock on the mutex.
- `wait_until` takes an absolute `steady_clock` time point, which is immune to wall-clock changes.

Natural extensions are repeating timers and running tasks on the thread pool from exercise 6.
