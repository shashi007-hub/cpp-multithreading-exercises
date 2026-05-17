# Exercise 17: False Sharing and thread_local

**Level:** Advanced  
**Paradigm:** Performance / data layout  
**Concepts:** cache lines, `alignas`, `thread_local`, sharded counters

## Problem

Make a sharded counter fast.

1. Give `PaddedSlot` its own cache line using `alignas(kCacheLine)`.
2. Implement `this_thread_index()`: every thread receives a stable, unique index the first time it asks, using `thread_local`.

The benchmark increments counters from four threads using packed versus padded slots and prints both timings. Both must produce the right total, but one should be dramatically faster.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-17`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-17`).

## Hints

Try the exercise first; open these one at a time.

1. `alignas(64)` on the struct also pads `sizeof` to a multiple of 64.
2. A `thread_local` variable is initialised once per thread: `thread_local const size_t index = next.fetch_add(1);`.
3. The shard index is `this_thread_index() % shards`.
4. On Apple silicon the cache line is 128 bytes; 64 is still a big win, 128 is safer.

## Solution walkthrough

Two threads writing *different* variables that share a cache line still fight over that line: each write invalidates the other core's copy. This is **false sharing**, and it can slow code down by 10-20x with no logical contention at all.

- `PackedSlot`s sit side by side (8 bytes each, eight per line). Padding each slot to a full line gives each thread exclusive ownership of its line.
- `thread_local` provides per-thread storage; here it caches a unique shard index so threads do not need to pass IDs around.
- Relaxed `fetch_add` suffices because the counter is just a statistic; `total()` is an approximate snapshot while writers run and exact after they join.

Take-away: correctness and performance are different axes. Measure with real hardware and look at data layout.
