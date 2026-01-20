# Exercise 14: Pipeline Pattern

**Level:** Intermediate  
**Paradigm:** Pipeline / stream processing  
**Concepts:** stages connected by queues, close propagation, throughput

## Problem

Build a three-stage pipeline: `string` lines, parsed to `int`, squared to `long long`.

Implement `make_stage(input, output, transform)`, a thread that moves items from one queue to the next and closes the output queue when the input is exhausted. Then wire two stages together in `run_pipeline` and verify that order is preserved and an empty input terminates.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-14`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-14`).

## Hints

Try the exercise first; open these one at a time.

1. A stage loops `while (auto item = input.pop())`.
2. Closing the output queue at the end is what lets the *next* stage finish. Close propagation replaces sentinel values.
3. The `jthread` objects must outlive the loop that drains the last queue.
4. Types flow: `string -> int -> long long`; each stage is a different template instantiation.

## Solution walkthrough

A pipeline overlaps work: while stage 2 processes item *n*, stage 1 already parses item *n+1*. Throughput approaches that of the slowest stage rather than the sum of all stages.

- Each stage is one thread plus an input queue. Single-threaded stages preserve ordering for free.
- The close signal cascades: `raw.close()` ends stage 1, which closes `parsed`, which ends stage 2, which closes `squared`, which ends the drain loop. No poison pills, no counters.
- `std::jthread` joins on destruction, so leaving the function is always safe.

Real uses: video decoding, log processing, compilers (lex, parse, optimise, emit), ETL jobs.

## Bonus challenges

- Allow a stage to run with several worker threads and restore order with sequence numbers.
- Bound the queues (exercise 4) so a slow stage applies back-pressure upstream.
- Add per-stage timing and identify the bottleneck stage automatically.
