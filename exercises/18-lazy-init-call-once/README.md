# Exercise 18: Thread-Safe Lazy Initialisation

**Level:** Intermediate  
**Paradigm:** Singleton / initialisation patterns  
**Concepts:** `std::call_once`, double-checked locking, function-local statics

## Problem

Implement three correct ways to initialise something exactly once.

1. `LazyValue<T>` built on `std::once_flag` / `std::call_once`.
2. `DoubleCheckedSingleton<T>`: an acquire-load fast path, a mutex-protected slow path and a release store.
3. `meyers_singleton()`: a function-local static.

In each test sixteen threads race to the first access while the factory sleeps; the factory must run exactly once and everybody must see the same object.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-18`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-18`).

## Hints

Try the exercise first; open these one at a time.

1. `call_once` blocks other callers until the first finishes, and rethrows/retries if the initialiser throws.
2. In the double-checked version the pointer must be `std::atomic`; a plain pointer is a data race.
3. Re-check the pointer after taking the mutex.
4. Since C++11, initialisation of function-local statics is thread safe.

## Solution walkthrough

Lazy initialisation is a classic source of subtle bugs. The famous broken pattern is double-checked locking with a **non-atomic** pointer: another thread may see the non-null pointer before the object's constructor effects are visible.

- `call_once` is the simplest correct tool and should be your default.
- The atomic version is correct because the release store of the pointer happens-before the acquire load that observes it, so the constructed object is fully visible. The mutex keeps two threads from both constructing.
- A function-local static is the idiomatic one-liner for process-wide singletons; the compiler emits the guard for you.

Prefer dependency injection to global singletons where possible; they make testing and shutdown order harder.

## Bonus challenges

- Make `LazyValue` re-attempt initialisation when the factory throws and test it.
- Break `DoubleCheckedSingleton` on purpose with a plain pointer and run TSan.
- Implement `std::atomic<std::shared_ptr<T>>`-based lazy init and compare.
