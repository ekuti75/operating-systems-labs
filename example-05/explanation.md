# Example 05 – Thread Management (Windows API)

## Purpose
This exercise demonstrates the fundamental concept of multithreading using the native **Windows API** (`CreateThread` and `WaitForSingleObject`). Threads allow concurrent execution paths within the same process memory space, providing better resource sharing and performance than processes.

## Key Functions
- **CreateThread():** The core function used to create a new thread of execution. It returns a **handle** (`HANDLE`) to the new thread.
- **GetCurrentThreadId():** Retrieves the unique ID of the thread currently executing the code.
- **Sleep(ms):** Pauses the thread's execution for a specified duration in **milliseconds** (e.g., 2000 for 2 seconds).
- **WaitForSingleObject():** Used for synchronization. The main thread calls this function to **block** its own execution until the created thread (identified by its handle) terminates, similar to `pthread_join` in POSIX environments.

## Execution Flow
1. The main thread initializes the data (`value = 100`).
2. **`CreateThread`** starts the worker thread function concurrently.
3. The main thread is blocked at **`WaitForSingleObject`**.
4. The worker thread executes its task, waits for 2000 milliseconds, and then returns.
5. `WaitForSingleObject` detects the termination, allowing the main thread to resume, close the thread handle, and exit gracefully.