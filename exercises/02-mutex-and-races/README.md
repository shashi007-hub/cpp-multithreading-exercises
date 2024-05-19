# Exercise 02: Mutexes and Data Races

**Level:** Beginner  
**Paradigm:** Shared-memory mutual exclusion  
**Concepts:** `std::mutex`, `std::lock_guard`, critical sections, check-then-act races

## Problem

The starter `Account` class is not thread safe. Make it correct.

- `deposit` and `withdraw` must be atomic with respect to each other.
- `withdraw` must refuse (return `false`) when funds are insufficient, and the balance must never become negative even with many concurrent withdrawers.
- `balance()` must be callable on a `const` account.

The tests hammer the account from eight threads. Run them several times; a data race may pass by luck.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-02`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-02`).

## Hints

Try the exercise first; open these one at a time.

1. A single `std::mutex` member protects all state that must change together.
2. `withdraw` is a *check-then-act* sequence. The check and the update need the same critical section.
3. To lock inside a `const` method the mutex must be `mutable`.
4. Use `std::lock_guard` (or `std::scoped_lock`) so the mutex is released on every return path.

## Solution walkthrough

A data race is two threads touching the same memory without synchronisation where at least one writes. `balance_ += amount` is a read-modify-write, so concurrent deposits overwrite each other.

- `std::lock_guard` ties unlock to scope exit (RAII), which makes early returns and exceptions safe.
- Locking only the update inside `withdraw` is **not enough**: two threads can both pass the `balance_ < amount` test and both subtract. The invariant "balance never negative" spans the check *and* the update, so both live in one critical section.
- Reading `balance_` without the lock is still a data race (and undefined behaviour), even if it "looks" harmless.

Rule of thumb: lock around invariants, not around individual variables.
