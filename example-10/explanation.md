# Example 10 – Deadlock Simulation (Circular Wait)

## Purpose
This exercise simulates a classic **Deadlock** scenario. Deadlock occurs when a set of processes/threads are permanently blocked because each is waiting for a resource held by another process in the set. The simulation demonstrates the **Circular Wait** condition, one of the four necessary conditions for deadlock.

## Deadlock Conditions (Necessary)
1. **Mutual Exclusion:** Resources (Mutexes) are non-shareable.
2. **Hold and Wait:** Threads hold one resource while waiting for another. (T1 holds A, waits for B; T2 holds B, waits for A).
3. **No Preemption:** Resources cannot be forcibly taken away.
4. **Circular Wait:** A closed chain of threads exists, where each thread waits for a resource held by the next thread in the chain (T1 waits for T2, T2 waits for T1).

## Execution Logic
- **Resource Request:** Thread 1 requests A then B. Thread 2 requests B then A.
- **Deadlock:** Due to the `Sleep(500)` call, it's highly likely that T1 will successfully lock A, and T2 will successfully lock B. When they subsequently request the resource held by the other, they become permanently blocked.
- **Detection:** The `WaitForMultipleObjects` call is intentionally given a short timeout (5000ms). If the threads don't finish in time, the program assumes a deadlock occurred.