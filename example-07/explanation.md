# Example 07 – Thread Synchronization using Semaphore (Windows API)

## Purpose
This exercise demonstrates the use of a **Semaphore** for thread synchronization. Unlike a Mutex, which allows only one thread access (count of 1), a Semaphore controls access to a limited pool of resources, allowing a specified number of threads to enter a critical section concurrently.

## Key Functions
- **CreateSemaphore():** Creates the Semaphore object, initializing its current count and maximum count (MAX_THREADS_ALLOWED).
- **WaitForSingleObject(Semaphore, INFINITE):** Used to acquire a resource unit from the Semaphore. This call **decrements** the semaphore count and blocks if the count is zero.
- **ReleaseSemaphore(Semaphore, 1, NULL):** Used to release a resource unit. This call **increments** the semaphore count, potentially waking up a waiting thread.
- **WaitForMultipleObjects():** Used to wait for all threads to terminate (cleanup).

## Execution Flow
We create 5 threads, but the Semaphore is initialized to a count of 2. 
1. Only the first two threads (T1 and T2) can acquire the semaphore and enter the resource.
2. The remaining threads (T3, T4, T5) are blocked at `WaitForSingleObject`.
3. When T1 or T2 calls `ReleaseSemaphore`, the count increases, allowing one of the blocked threads to proceed.