# Non-blocking vs Blocking:

An algorithm is called non-blocking if the failure or suspension of any thread cannot cause the
failure or suspension of another thread.

The main categories for non-blocking algorithms are lock-free and wait-free. Each wait-free algorithm is lock-free,
and each lock-free is non-blocking. Non-blocking and lock-free are not the same.

Non-blocking algorithms are typically implemented with _CAS_ instructions. CAS stands for compare and swap. CAS is
called `compare_exchange_strong` or `compare_exchange_weak` in C++.

## Lock-free & WaitFree

A non-blocking algorithm is considered lock-free when it ensures system-wide progress. It guarantees that at least one
thread will make progress within a finite number of steps (operations), even if other threads may face contention or
delays. If a thread accessing the data structure is suspended by the scheduler during its operation, other threads
should continue their operations without waiting for the suspended thread.

**Wait-free**: A non-blocking algorithm is wait-free if there is guaranteed per-thread progress.

# Resources:

[Parallel C++: Blocking and Non-Blocking Algorithms](https://youtu.be/Uh6wXoXydAg?si=HHnbf2g8d67wiviM)

[Parallel C++: Lock-Free and Wait-Free Algorithms](https://youtu.be/USkke1-SZE8?si=6Ay-n6jJafUKHjdL)