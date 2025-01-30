# Memory Order in Five Minutes

| Order | Guarantee | Typical use |
| --- | --- | --- |
| `relaxed` | Atomicity only; no ordering with other memory | Counters, statistics |
| `acquire` (loads) | Later reads/writes cannot move before it | Lock, consuming a flag |
| `release` (stores) | Earlier reads/writes cannot move after it | Unlock, publishing data |
| `acq_rel` | Both, for read-modify-write | `fetch_add` hand-offs |
| `seq_cst` | One global order of all `seq_cst` operations | Default; use when unsure |

## The one pattern to remember

```cpp
// writer
payload = 42;                                   // ordinary write
ready.store(true, std::memory_order_release);   // publish

// reader
while (!ready.load(std::memory_order_acquire)) {}
use(payload);                                   // guaranteed to see 42
```

A release store **synchronises-with** an acquire load that reads the value it wrote. Everything the writer did
before the store is visible to the reader after the load.

## Rules of thumb

1. Start with `seq_cst` and a mutex. Optimise after measuring.
2. A mutex lock is an acquire and its unlock a release, which is why locked data needs no atomics.
3. `relaxed` is only correct when nothing else is published through that atomic.
4. Verify weaker orders with ThreadSanitizer and, for real confidence, a model checker such as CDSChecker.
