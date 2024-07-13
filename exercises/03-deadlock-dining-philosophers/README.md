# Exercise 03: Deadlock and the Dining Philosophers

**Level:** Beginner  
**Paradigm:** Lock ordering / deadlock avoidance  
**Concepts:** deadlock conditions, `std::scoped_lock`, lock ordering

## Problem

Five philosophers sit around a table with five forks. Each needs the fork on their left and right to eat. The naive loop (lock left, then lock right) deadlocks when everybody grabs their left fork at once.

Fill in the lock acquisition so that every philosopher eats `meals` times, no two philosophers ever hold the same fork, and the program never hangs. It must also work with just two philosophers.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-03`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-03`).

## Hints

Try the exercise first; open these one at a time.

1. Deadlock needs a cycle of waiting threads. Break the cycle and it cannot happen.
2. C++17 gives you `std::scoped_lock(a, b)`, which locks several mutexes with a deadlock-avoidance algorithm.
3. The alternative is a global order: always lock the lower-numbered fork first.
4. Keep the lock object alive for the whole meal (declare it before `eat`).

## Solution walkthrough

Deadlock requires four conditions (Coffman): mutual exclusion, hold-and-wait, no pre-emption and **circular wait**. The cheapest one to remove is circular wait.

`std::scoped_lock both(left.mutex, right.mutex)` uses `std::lock`'s try-and-back-off algorithm, so a thread never blocks on one mutex while holding another in a way that can form a cycle. The exclusivity check (`in_use` flags in `eat`) proves the forks were really mutually exclusive.

Other valid fixes: impose a total order on forks (lock `min(i, j)` first), or limit the table to `N - 1` diners with a semaphore. The order approach is the one used in production code because it needs no special primitive.

## Bonus challenges

- Implement the lock-ordering variant with plain `std::lock_guard` and compare the throughput.
- Write a version that deadlocks on purpose and detect it with a watchdog thread plus a timeout.
- Switch to `std::timed_mutex` and `try_lock_for`. What new problem (livelock/starvation) appears?
