# Example 01 – Simple System Call Demonstration

## Purpose
The purpose of this exercise is to implement a basic program using a low-level system call (like 'write') to understand the direct interaction between a user program and the operating system kernel.

## System Calls / Functions Used
- write()
- main()

## How It Works
The program calls the 'write' system call directly, passing the file descriptor for standard output (1), the text string, and the length of the string. The kernel handles displaying the message to the terminal.

## How to Compile and Run
Use the following commands:
$ gcc main.c -o app
$ ./app