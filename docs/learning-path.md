# Learning Path

The exercises are ordered so that each one needs only ideas from the earlier ones.

| Stage | Exercises | You learn |
| --- | --- | --- |
| 1. Foundations | 1-3 | Starting and joining threads, data races, mutexes, deadlock |
| 2. Coordination | 4-5, 12 | Condition variables, closing queues, latches, barriers, semaphores |
| 3. Structured concurrency | 6-7, 16 | Thread pools, futures, promises, fork-join |
| 4. Shared-state scaling | 10-11, 13, 17 | Reader-writer locks, striping, map-reduce, false sharing |
| 5. Lock-free | 8-9 | Atomics, memory ordering, SPSC queues |
| 6. Architecture | 14-15, 18-20 | Pipelines, actors, lazy init, cancellation, schedulers |

Suggested pace: one exercise per sitting. Rerun your solution at least twenty times and once under
ThreadSanitizer (see [tooling.md](tooling.md)) before calling it done.

After the last exercise, re-implement two of them from memory without the starter. If you can explain *why*
each synchronisation point exists, you understand it.
