# Exercise 19: Cooperative Cancellation with stop_token

**Level:** Advanced  
**Paradigm:** Cancellation / lifecycle management  
**Concepts:** `std::jthread`, `std::stop_token`, `std::stop_source`, `condition_variable_any`

## Problem

Add cancellation to two components.

1. `PeriodicTask` runs a callback every `period` on its own thread. Destroying it must stop the thread immediately, even if the period is an hour.
2. `parallel_find(values, target, workers)` searches in parallel and makes the other workers stop as soon as one finds the target, using a shared `std::stop_source`.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-19`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-19`).
