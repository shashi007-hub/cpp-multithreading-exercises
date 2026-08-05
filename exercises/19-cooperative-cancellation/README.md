# Exercise 19: Cooperative Cancellation with stop_token

**Level:** Advanced  
**Paradigm:** Cancellation / lifecycle management  
**Concepts:** `std::jthread`, `std::stop_token`, `std::stop_source`, `condition_variable_any`

## Problem

Add cancellation to two components.

1. `PeriodicTask` runs a callback every `period` on its own thread. Destroying it must stop the thread immediately, even if the period is an hour.
2. `parallel_find(values, target, workers)` searches in parallel and makes the other workers stop as soon as one finds the target, using a shared `std::stop_source`.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-19`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-19`).

## Hints

Try the exercise first; open these one at a time.

1. `condition_variable_any::wait_for(lock, stop_token, period, predicate)` returns early when stop is requested.
2. The predicate is `[&]{ return stop.stop_requested(); }`; `false` from `wait_for` means "timed out, run the task".
3. `std::jthread` passes a `stop_token` to its callable and requests stop in its destructor.
4. Record the first hit with a compare-exchange so only one worker wins.

## Solution walkthrough

Killing a thread is not safe in C++. The only robust approach is **cooperative**: ask the thread to stop and let it exit at a safe point.

- `std::stop_token` is the read side, `stop_source` the write side, `jthread` owns one and joins automatically.
- Plain `sleep_for` ignores cancellation. Waiting on a `condition_variable_any` with the token wakes immediately on a stop request, so shutdown latency becomes microseconds, not one period.
- In `parallel_find`, each worker polls `stop_requested()` in its loop. A cheap atomic check per element is fine; for very hot loops check every N iterations.
- Having `~PeriodicTask` request stop and then letting `jthread` join gives deterministic shutdown with no dangling thread.
