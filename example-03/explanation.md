# Example 03 – Parent Process Waiting (Synchronization)

## Purpose
This exercise demonstrates process synchronization, specifically how a parent process waits for its child process to complete execution before the parent continues its own execution. This prevents the parent from terminating early and leaving the child running (a zombie state in Unix, handled differently in Windows).

## Functions Used
- CreateProcess(): Creates the child process.
- GetCurrentProcessId(): Gets the Parent's PID.
- WaitForSingleObject(): The core function that blocks the parent process until the object (the child process handle) is signaled (i.e., the child process terminates).

## How It Works
After successfully creating the child process (cmd.exe), the parent calls WaitForSingleObject(pi.hProcess, INFINITE). This call pauses the parent's execution until the user manually closes the 'cmd.exe' window. Once the child is closed, the parent resumes, printing the final termination message.