# Exercise 13: Map-Reduce Word Count

**Level:** Intermediate  
**Paradigm:** Data parallelism (map/reduce)  
**Concepts:** thread-local aggregation, merging, avoiding shared writes

## Problem

Count word frequencies in a large corpus in parallel.

Implement `parallel_word_count(lines, workers)`:

- **Map**: each worker counts words in its own slice of lines into a private `WordCounts` map.
- **Reduce**: after joining, merge the private maps into the final result.

The output must equal the sequential `count_words` for any worker count, including more workers than lines.

Work in [`starter.cpp`](starter.cpp) and check yourself with `make test-13`. The reference is in [`solution.cpp`](solution.cpp) (`make solution-13`).

## Hints

Try the exercise first; open these one at a time.

1. No locks should be necessary: workers share only read-only input.
2. Reuse `count_words(lines, begin, end)` for the map step.
3. Clamp the slice bounds exactly as in exercise 1.
4. Merging can happen on the calling thread; it is O(distinct words) rather than O(total words).
