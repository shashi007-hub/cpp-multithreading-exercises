# Standard Library Cheat Sheet

| Header | Facility | Use it for |
| --- | --- | --- |
| `<thread>` | `std::thread`, `std::jthread` | Starting threads; prefer `jthread` |
| `<mutex>` | `mutex`, `recursive_mutex`, `timed_mutex`, `lock_guard`, `unique_lock`, `scoped_lock`, `once_flag` | Mutual exclusion and one-time init |
| `<shared_mutex>` | `shared_mutex`, `shared_lock` | Many readers, one writer |
| `<condition_variable>` | `condition_variable`, `condition_variable_any` | Waiting for state changes |
| `<future>` | `future`, `promise`, `packaged_task`, `async`, `shared_future` | Results from asynchronous work |
| `<atomic>` | `atomic<T>`, `atomic_flag`, `atomic_ref` | Lock-free shared variables |
| `<latch>`, `<barrier>` | `latch`, `barrier` | Group synchronisation |
| `<semaphore>` | `counting_semaphore`, `binary_semaphore` | Permits and signalling |
| `<stop_token>` | `stop_source`, `stop_token`, `stop_callback` | Cooperative cancellation |
| `<execution>` | `std::execution::par` | Parallel algorithms (library support varies) |

## Which lock wrapper?

| Need | Use |
| --- | --- |
| Lock one mutex for a scope | `std::lock_guard` |
| Lock several mutexes safely | `std::scoped_lock` |
| Unlock early or use with a condition variable | `std::unique_lock` |
| Read-only access to a `shared_mutex` | `std::shared_lock` |
