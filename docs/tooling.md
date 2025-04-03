# Tooling

## ThreadSanitizer (TSan)

Detects data races at runtime.

```sh
make tsan-08
```

Notes:

- Run with several iterations; TSan only reports races that actually occur.
- It may report false positives for `std::barrier` and `std::latch` on some standard libraries.
- It slows programs by 5-15x and needs more memory.

## AddressSanitizer and UBSan

```sh
make asan-06
```

Finds use-after-free and out-of-bounds accesses, the usual outcome of dangling thread captures.

## Helgrind / DRD (Valgrind, Linux)

```sh
valgrind --tool=helgrind ./build/solution-03
```

Good at lock-order inversion detection, which TSan only partially covers.

## Profiling

- `perf stat -e cache-misses` for false sharing hints (Linux).
- Instruments > Time Profiler and System Trace (macOS).
- Compare `real` and `user` time with `time` to see how much parallelism you actually obtained.

## Stress techniques

- Run the test in a loop: `for i in $(seq 100); do ./build/starter-02 || break; done`.
- Add `std::this_thread::yield()` or short sleeps at suspicious points to widen race windows.
- Use more threads than cores.
