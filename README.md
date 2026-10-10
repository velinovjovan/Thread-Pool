# ThreadPool

A C++ thread pool implementation designed for efficient task scheduling and concurrency management using standard library primitives.

## Overview

The `ThreadPool` class manages a fixed pool of worker threads that execute tasks concurrently from a shared queue. It utilizes a producer-consumer pattern synchronized via `std::mutex` and `std::condition_variable` to handle safe task submission and thread coordination.

## Core Components

* **Worker Management**: Spawns and maintains a configurable vector of worker threads upon initialization, cleaning them up cleanly via RAII in the destructor.
* **Task Queue**: Stores pending tasks in a `std::queue` using type-erased `std::function` wrappers.
* **Synchronization**: Protects the shared task queue with a mutual exclusion lock (`std::mutex`) and coordinates worker wake-ups using a `std::condition_variable`.
