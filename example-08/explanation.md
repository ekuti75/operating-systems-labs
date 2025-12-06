# Example 08 – Synchronization using Condition Variables (Producer-Consumer)

## Purpose
This exercise demonstrates a solution to the classic Producer-Consumer problem using **Condition Variables (CVs)**, a critical tool for advanced thread synchronization. CVs allow threads to efficiently wait for a condition (e.g., buffer becoming empty or full) to become true, rather than constantly polling (busy waiting).

## Key Functions (Windows API)
- **CRITICAL_SECTION:** Used here instead of a Mutex for protecting the shared buffer (`buffer_lock`). It's a lightweight, faster form of mutual exclusion within a single process.
- **CONDITION_VARIABLE:** The primary object used for waiting and signaling:
    - **InitializeConditionVariable():** Initializes the CV.
    - **SleepConditionVariableCS():** Atomically releases the critical section (lock) and waits for the condition variable to be signaled. This is the waiting mechanism.
    - **WakeConditionVariable():** Wakes up one waiting thread associated with the condition variable.

## Execution Flow
1. Threads acquire `buffer_lock`.
2. Threads check their respective condition (`is_empty` or `!is_empty`).
3. If the condition is *not met*, the thread calls `SleepConditionVariableCS()`, which releases the lock and pauses the thread.
4. After state change (Producer fills, Consumer empties), the thread calls `WakeConditionVariable()` to notify the waiting counterpart, ensuring efficient resource management.