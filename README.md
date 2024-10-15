# C++ Multithreading Exercises

A hands-on course in concurrent programming with modern C++ (C++20). Twenty graded exercises take you from
`std::thread` to lock-free queues and a timer scheduler, covering the main concurrency paradigms and the
patterns used in real systems.

Each exercise has:

- a problem statement, hints, a full solution walkthrough and bonus challenges (`README.md`),
- a `starter.cpp` with `TODO`s and self-checking tests that you complete,
- a reference `solution.cpp` that passes the same tests.


## How to use this repository

Requirements: a C++20 compiler (Clang 15+ or GCC 12+) and `make`.

```sh
make test-01        # build and run YOUR exercises/01-*/starter.cpp
make solution-01    # build and run the reference solution
make all-solutions  # run every reference solution
```

Suggested loop for each exercise:

1. Read the problem in `exercises/NN-name/README.md`.
2. Edit `starter.cpp` until `make test-NN` passes. Run it repeatedly; concurrency bugs are probabilistic.
3. Only then read the hints, and finally the walkthrough, and compare with `solution.cpp`.
4. Try the bonus challenges.


## Exercises

| # | Exercise | Level | Paradigm |
| --- | --- | --- | --- |
| 1 | [Hello, Threads](exercises/01-hello-threads) | Beginner | Thread-per-task / data parallelism |
| 2 | [Mutexes and Data Races](exercises/02-mutex-and-races) | Beginner | Shared-memory mutual exclusion |
| 3 | [Deadlock and the Dining Philosophers](exercises/03-deadlock-dining-philosophers) | Beginner | Lock ordering / deadlock avoidance |
| 4 | [Bounded Buffer (Producer/Consumer)](exercises/04-bounded-buffer) | Beginner | Monitor / condition variables |
| 5 | [Blocking Queue with Close](exercises/05-blocking-queue) | Intermediate | Message passing / graceful shutdown |
| 6 | [Thread Pool](exercises/06-thread-pool) | Intermediate | Task-based concurrency / executor |
| 7 | [Futures, Promises and async](exercises/07-futures-and-promises) | Intermediate | Asynchronous results / dataflow |
