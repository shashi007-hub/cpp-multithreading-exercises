# How to Practise

1. **Predict before running.** Write down what could go wrong with your design and why.
2. **Run it many times.** `for i in $(seq 50); do make test-04 || break; done`.
3. **Break it deliberately.** Remove a lock, swap a memory order, drop a notify; confirm the tests (or TSan)
   catch it. If nothing fails, your test is too weak.
4. **Measure.** Use the `Stopwatch` helper. Compare against a single thread and against a naive lock.
5. **Explain it aloud.** For every mutex, name the invariant it protects. For every atomic, name the
   happens-before edge it creates.
6. **Re-implement from memory** a week later.
7. **Review other people's code** with a checklist: ownership, lock scope, shutdown, exception safety.
