# Introduction

Multithreading is a powerful technique for improving software performance by leveraging the capabilities of modern
processors with multiple cores. However, with this power comes the potential for certain performance bottlenecks, and
one such challenge is "False Sharing." In this guide, we'll dive deep into what false sharing is, why it's a problem,
how to detect it, and strategies to mitigate its impact effectively.

## Cache Basics: What is Cache and How It Works

Before we delve into false sharing, it's crucial to understand the basics of cache:

Cache: A high-speed, small-sized memory unit that stores frequently accessed data to reduce the need for slower main
memory access. Caches work by maintaining copies of data that are likely to be accessed soon.
Caches are an essential component of modern processors because they help bridge the speed gap between the CPU and main
memory.

## Cache Coherence and Cache Invalidation

To understand false sharing, we need to grasp the concept of cache coherence and cache invalidation:

Cache Coherence: Cache coherence is the principle that ensures all caches in a multiprocessor system have consistent
views of shared data. Without cache coherence, the same data in different caches could have different values, leading to
unpredictable behavior.

Cache Invalidation: When one core modifies data in its cache, it must notify other cores to invalidate or update their
copies of that data. This process is essential to maintain a consistent view of memory across all cores.

## What is a Cache Line

To comprehend false sharing fully, let's explore cache lines:

Cache Line: A cache line is a fixed-size block of data in a cache, typically 64 or 128 bytes in size. It is the smallest
unit that can be loaded into or evicted from a cache. When you read or write data, you usually fetch or modify an entire
cache line, not just the specific data element you need.
Understanding cache lines is crucial because it directly relates to how false sharing occurs and its impact.

## False Sharing Explained

Now, let's tackle false sharing in detail:

What is False Sharing: False sharing occurs when multiple threads, running on different processor cores, inadvertently
modify different parts of the same cache line. Importantly, these threads are not modifying the same data; however, the
cache coherence protocol treats it as if they are.

Impact of False Sharing: False sharing can have a severe impact on performance. When threads share the same cache line,
any modification to that line by one thread leads to cache invalidation for all other threads accessing that cache line.
This results in unnecessary synchronization overhead, reduced parallelism, and slower program execution.

## Detecting False Sharing

Use `perf` on linux or `Instruments` on macos to detect false sharing. Check the cache misses of your application, 
and if it is a large number, this means that there is a lot of cache contention.

## Mitigating False Sharing

Once you've identified false sharing, it's essential to mitigate its impact:

1. Padding Data Structures: One effective way to mitigate false sharing is to ensure that data structures are correctly
padded. This means adding padding elements to align the data structure's size with cache line boundaries. Padding
ensures that unrelated data does not share a cache line, reducing the chance of false sharing.

2. `alignon` in C++
```c++
/**
 * `alignas(128)` says that the variable is stored at the beginning of a cache line.
 * This makes sure that no two counters share the same cache line.
 * You can know the size of the cache line using `sysctl -a | grep cache`
 */
struct AlignedAtomic {
  alignas(64) std::atomic<int> counter{0};
};
```

3. Thread-Local Storage: Another strategy is to use thread-local storage (TLS) for frequently accessed data. TLS ensures
that each thread has its own copy of the data, eliminating the possibility of false sharing entirely.