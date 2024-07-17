# Concurrency Paradigms

## Shared memory with locks
Threads share data and take turns using mutexes. Simple and ubiquitous; hard to scale and compose.
Exercises 2, 3, 10, 11.

## Condition-based coordination (monitors)
A mutex plus condition variables express "wait until the state is right". Exercises 4, 5.

## Message passing
Threads share nothing and exchange messages through queues. Channels, pipelines and actors all live here.
Exercises 5, 14, 15.

## Task-based parallelism
Describe *tasks*; let an executor schedule them. Futures carry results. Exercises 6, 7, 16.

## Data parallelism
Split data, apply the same operation to each part, combine. Exercises 1, 13.

## Lock-free programming
Coordinate with atomic instructions instead of locks. Fast and fragile; use for narrow, well-understood
structures. Exercises 8, 9.

## Structured and cooperative concurrency
Tie thread lifetimes to scopes and cancel through tokens rather than killing threads. Exercises 18, 19, 20.

## Choosing a paradigm

| If you have... | Prefer |
| --- | --- |
| Independent chunks of numeric work | Data parallelism or a thread pool |
| A shared cache or table | Reader-writer lock or striping |
| Stages that transform a stream | Pipeline |
| A stateful component with many clients | Actor |
| Latency-critical hand-off between two threads | SPSC ring buffer |
| Things that must happen at a later time | Timer scheduler |
