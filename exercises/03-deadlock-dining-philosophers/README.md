# Exercise 03: Deadlock and the Dining Philosophers

**Level:** Beginner  
**Paradigm:** Lock ordering / deadlock avoidance  
**Concepts:** deadlock conditions, `std::scoped_lock`, lock ordering

## Problem

Five philosophers sit around a table with five forks. Each needs the fork on their left and right to eat. The naive loop (lock left, then lock right) deadlocks when everybody grabs their left fork at once.

Fill in the lock acquisition so that every philosopher eats `meals` times, no two philosophers ever hold the same fork, and the program never hangs. It must also work with just two philosophers.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-03`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-03`).
