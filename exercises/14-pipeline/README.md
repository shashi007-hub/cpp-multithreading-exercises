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
