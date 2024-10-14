# Pattern Catalogue

| Pattern | Problem it solves | Core tools | Exercise |
| --- | --- | --- | --- |
| Producer-consumer | Decouple producers from consumers, add back-pressure | mutex, condition variables | 4 |
| Channel / blocking queue | Hand-off with graceful shutdown | close semantics | 5 |
| Thread pool / executor | Bound thread count, reuse threads | queue, packaged_task | 6 |
| Future / promise | Return a result from asynchronous work | `std::future` | 7 |
| Reader-writer lock | Many readers, few writers | `std::shared_mutex` | 10 |
| Lock striping | Reduce contention on a shared map | array of mutexes | 11 |
| Barrier (bulk-synchronous) | Phases that need all threads | `std::barrier` | 12 |
| Map-reduce | Aggregate large data with no shared writes | private maps, merge | 13 |
| Pipeline | Overlap stages of a stream | queues | 14 |
| Active object / actor | Serialise access by confinement | mailbox thread | 15 |
| Fork-join | Divide and conquer | `std::async`, cutoff | 16 |
| Sharding / padding | Avoid false sharing | `alignas`, `thread_local` | 17 |
| Once-only initialisation | Safe lazy singletons | `call_once` | 18 |
| Cooperative cancellation | Stop work promptly and safely | `stop_token` | 19 |
| Timer wheel / delay queue | Run work later | priority queue, timed wait | 20 |
| Monitor object | Bundle a mutex with its data | class with private mutex | 2 |
| Double-checked locking | Cheap read path for lazy init | atomics + mutex | 18 |
| Thread confinement | Avoid sharing | thread_local, actor | 15, 17 |
