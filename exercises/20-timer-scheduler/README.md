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
