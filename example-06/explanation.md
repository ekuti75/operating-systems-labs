# Example 06 – Thread Synchronization using Mutex (Windows API)

## Purpose
This exercise demonstrates the fundamental concept of thread synchronization using a **Mutex (Mutual Exclusion)**. The goal is to safely manage access to a **critical section** (a shared resource, `global_counter`) by multiple threads, preventing **race conditions** and ensuring the final result is correct.

## Key Functions (Windows API Synchronization Primitives)
- **CreateMutex():** Creates a synchronization object (the Mutex) that can be owned by only one thread at a time.
- **WaitForSingleObject(Mutex, INFINITE):** Used to acquire (lock) the Mutex. The calling thread blocks here until the Mutex is available.
- **ReleaseMutex(Mutex):** Releases the Mutex, allowing another waiting thread to acquire it.
- **WaitForMultipleObjects():** Used by the main thread to wait for all worker threads to terminate simultaneously (a parallel 'join').

## Race Condition vs. Mutex Solution
Without the Mutex, the threads would overwrite each other's updates to `global_counter`, resulting in an incorrect final value (a race condition). By using `WaitForSingleObject` and `ReleaseMutex`, we ensure that only one thread modifies the counter at any given time, guaranteeing the final count is equal to THREAD_COUNT * INCREMENT_COUNT.