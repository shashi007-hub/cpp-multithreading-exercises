# Where These Patterns Appear in Real Systems

| Pattern | Real-world examples |
| --- | --- |
| Thread pool | Web servers (Nginx workers, Tomcat), `ThreadPoolExecutor`, game job systems |
| Producer-consumer / queue | Kafka consumers, logging back-ends, work queues in Celery or Sidekiq |
| Pipeline | FFmpeg filter graphs, compilers, ETL tools, Unix pipes |
| Actor | Erlang/OTP, Akka, Orleans, per-connection state machines |
| Reader-writer cache | DNS caches, configuration stores, in-memory lookup tables |
| Lock striping | `ConcurrentHashMap`, database lock managers, memcached item locks |
| SPSC ring buffer | Audio callbacks, NIC drivers, LMAX Disruptor, `tracing` back-ends |
| Barrier phases | Physics and fluid simulation, MPI collectives, parallel graph algorithms |
| Map-reduce | Hadoop, Spark, build systems aggregating results |
| Fork-join | TBB, Java ForkJoinPool, OpenMP tasks, `std::sort` with `par` |
| Cancellation tokens | .NET `CancellationToken`, Go `context`, structured concurrency libraries |
| Timer scheduler | UI toolkits, game loops, gRPC deadlines, TCP retransmission timers |
| Lazy init | Logging singletons, JIT caches, plugin registries |
| False-sharing avoidance | Per-CPU counters in the Linux kernel, metrics libraries |
