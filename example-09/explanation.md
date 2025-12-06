# Example 09 – Synchronization: Readers-Writers Problem

## Purpose
This exercise solves the **Readers-Writers Problem**, a classic synchronization challenge where multiple threads (readers and writers) share a resource. The solution must ensure:
1. Multiple **Readers** can access the resource concurrently.
2. Only **one Writer** can access the resource at a time.
3. No Reader can access the resource while a Writer is active.

## Synchronization Primitives (Windows API)
- **rw_mutex (Mutex):** Acts as the primary lock. It is used to protect the **writers** and prevent any access during writing.
- **read_count_mutex (Mutex):** A specialized lock used *only* to protect the shared variable `read_count`.
- **read_count (Integer):** Tracks the number of readers currently in the critical section.

## Execution Logic
1. **Reader Entry:** A reader acquires `read_count_mutex`, increments `read_count`. If `read_count` becomes 1 (first reader), it acquires `rw_mutex` to block all writers. It then releases `read_count_mutex`.
2. **Reader Exit:** The reader acquires `read_count_mutex`, decrements `read_count`. If `read_count` becomes 0 (last reader), it releases `rw_mutex` to allow a writer to proceed.
3. **Writer Entry/Exit:** A writer simply acquires `rw_mutex` before writing and releases it afterward. This ensures exclusivity.