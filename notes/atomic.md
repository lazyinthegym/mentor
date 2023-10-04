# Understanding `std::atomic` in C++

## What is `std::atomic`?

- `std::atomic` ensures atomic operations, meaning they execute as a single transaction.
- Other threads see the system's state before or after, not in-between.
- Implemented using special hardware instructions.

## How `std. atomic` Works

- `std.atomic` provides a mechanism to safely modify shared data without the need for a mutex.
- It acts like operations inside a mutex-protected critical section but is often more efficient due to hardware support.
- Operations on `std::atomic` variables are designed to be performed directly by the CPU, using specialized atomic
  instructions provided by the hardware.

## Benefits Over Mutex

- **Efficiency**: Mutexes are general-purpose synchronization primitives that involve context switching, thread
  suspension, and OS involvement, making them relatively slow. In contrast, `std::atomic` operations are performed at a
  lower level, often involving a single atomic instruction, which is faster.

- **Fine-Grained Control**: Mutexes are typically used to protect larger sections of code, leading to contention among
  threads. `std::atomic` allows for finer-grained control, enabling you to protect only the specific data that needs
  synchronization. This reduces contention and can lead to better performance in scenarios with high concurrency.

- **No Deadlocks**: Mutexes can lead to deadlocks if not managed carefully. `std::atomic` operations don't introduce the
  possibility of deadlocks, making them easier to work with in complex multi-threaded applications.

- **Lower Overhead**: Mutexes involve overhead, such as memory allocation and management, which can affect performance,
  especially in high-frequency operations. `std::atomic` operations have lower overhead, making them suitable for
  performance-critical scenarios.

## Why Copy Operations are Deleted

- Copy operations for `std::atomic` are deleted for good reasons. Consider what would happen if the initialization
  of `auto y = x` compiled with `x` being a `std::atomic` variable.
    - If copy construction was supported, `y`'s type would be deduced to be `std::atomic` as well.
    - One of the best things about `std::atomic` types is that all their operations are atomic, but for copy
      construction of `y` from `x` to be atomic, compilers would have to generate code to read `x` and write `y` in a
      single atomic operation. Hardware generally can't do that, so copy construction isn't supported.
- Copy assignment is also deleted for the same reason, which is why the assignment from `x` to `y` won't compile.
- `std::atomic` offers neither move construction nor move assignment.

## What Types can be Made Atomic?

- Any trivially copyable type.
- Trivially copyable means continuous memory, all bits copied (memcpy), and no virtual functions or noexcept
  constructor.

## Resources

- Book: "Effective Modern C++, Item 40: Use `std::atomic` for concurrency, `volatile` for special memory."
- [CppCon 2017: Fedor Pikus “C++ atomics, from basic to advanced. What do they really do?”](https://youtu.be/ZQFzMfHIxng?si=Xg76HGz68yGNxW9v)
