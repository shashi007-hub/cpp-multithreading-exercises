# Glossary

- **Atomic operation**: an operation other threads observe as indivisible.
- **Critical section**: code that must not run concurrently with other critical sections guarding the same data.
- **Data race**: unsynchronised conflicting accesses to one memory location where at least one is a write. Undefined behaviour in C++.
- **Race condition**: a bug whose outcome depends on timing. Not every race condition is a data race.
- **Deadlock**: threads waiting for each other forever.
- **Livelock**: threads keep reacting to each other and make no progress.
- **Starvation**: a thread never gets the resource it needs.
- **Happens-before**: the ordering guarantee that makes one thread's writes visible to another.
- **Memory order**: the strength of ordering an atomic operation provides.
- **Lock-free**: some thread always makes progress, regardless of what others do.
- **Wait-free**: every thread makes progress in a bounded number of steps.
- **Spurious wake-up**: a wait returning without a notification.
- **Cache line**: the unit (usually 64 bytes) in which CPUs move memory; the cause of false sharing.
- **Back-pressure**: slowing producers when consumers cannot keep up.
- **Amdahl's law**: speedup is limited by the serial fraction of the program.
- **RAII**: tying resource lifetime (locks, threads) to object scope.
