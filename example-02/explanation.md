# Example 02 – Process Creation using fork()

## Purpose
This exercise demonstrates how to create a new, duplicated process (child process) from an existing process (parent process) using the 'fork()' system call. This is fundamental for understanding multiprocessing in operating systems.

## System Calls / Functions Used
- fork(): Creates a new child process.
- getpid(): Gets the Process ID (PID) of the current process.
- getppid(): Gets the Parent Process ID (PPID) of the current process.

## How It Works
The fork() call returns 0 to the child process, and the child's PID to the parent process. The code uses this return value (pid) to execute different code blocks for the parent and child processes, proving that two independent processes are now running.

## How to Compile and Run
Use the following commands:
$ gcc main.c -o app
$ ./app