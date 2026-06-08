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
