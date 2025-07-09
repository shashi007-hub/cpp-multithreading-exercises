# Exercise 11: Striped Concurrent Hash Map

**Level:** Intermediate  
**Paradigm:** Fine-grained locking  
**Concepts:** lock striping, contention, per-bucket invariants

## Problem

One global lock around a hash map serialises every operation. Implement `StripedMap<Key, Value, Stripes>` where keys are distributed over independent stripes, each with its own mutex.

Provide `insert_or_assign`, `find`, `erase`, an atomic `update(key, fn)` and `size()`. Operations on keys in different stripes must not block each other.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-11`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-11`).

## Hints

Try the exercise first; open these one at a time.

1. Pick the stripe with `std::hash<Key>{}(key) % Stripes`; the choice must be a pure function of the key.
2. `update` runs the callback while holding that stripe's lock, which makes read-modify-write atomic per key.
3. `size()` locks stripes one at a time. The total is a snapshot approximation, which is usually acceptable.
4. Never lock two stripes at once unless you always lock them in index order.

## Solution walkthrough

Striping partitions shared state so that unrelated operations do not contend. With 16 stripes and uniformly distributed keys, expected contention drops by about 16x.

- Each stripe is a self-contained map plus mutex, so correctness reasoning stays local: an operation only needs its own stripe.
- `update` is the key API. A separate `find` followed by `insert` would be a check-then-act race; a callback executed under the lock avoids it.
- `size()` and any whole-map operation touch all stripes. If they needed a *consistent* snapshot you would lock all stripes in a fixed order, which is the price of striping.

Java's `ConcurrentHashMap` (pre-8) and many database lock managers use the same idea.
