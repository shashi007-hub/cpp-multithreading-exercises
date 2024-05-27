# Concurrency Pitfalls

| Pitfall | Symptom | Fix |
| --- | --- | --- |
| Data race | Wrong totals, crashes, "works on my machine" | Protect with a mutex or make the variable atomic |
| Check-then-act | Invariant broken although every access is locked | Put check and action in one critical section |
| Lost wake-up | Thread sleeps forever | Change state under the mutex; wait with a predicate |
| Spurious wake-up | Occasional wrong behaviour after `wait` | Always use the predicate overload |
| Deadlock | Program hangs | One lock order everywhere, or `std::scoped_lock` |
| Holding a lock while calling unknown code | Deadlocks, latency spikes | Copy data out, unlock, then call |
| Forgetting to join | `std::terminate` | Use `std::jthread` or RAII wrappers |
| Dangling references | Random crashes | Capture by value when the thread outlives the scope |
| False sharing | Threads slow each other with no shared data | Pad or separate hot variables |
| Oversubscription | Slower with more threads | Size pools to the workload |
| Detached threads | Use-after-free at shutdown | Avoid `detach`; own and join your threads |
| Blocking in a pool task that waits on another pool task | Pool deadlock | Never wait on work queued behind you |
| Relying on `volatile` | Undefined behaviour | `volatile` is not a synchronisation tool; use atomics |
| Assuming `relaxed` is "fast `seq_cst`" | Rare, unreproducible bugs | Use acquire/release only when you can prove it |
