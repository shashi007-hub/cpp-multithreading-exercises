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
