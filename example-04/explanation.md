# Example 04 – Inter-Process Communication (IPC) using Windows Pipes

## Purpose
This exercise demonstrates the creation and use of a basic Pipe for Inter-Process Communication (IPC) between a parent process and its child process within the Windows environment. This technique allows one-way data flow from the Parent to the Child process.

## Functions Used
- CreatePipe(): Creates an anonymous pipe, returning two handles: one for reading and one for writing.
- CreateProcess(): Creates the child process. Crucially, we set the child's standard input (stdin) to be the read end of the pipe.
- WriteFile(): The parent process uses this to send data (a string message) into the pipe's write end.
- WaitForSingleObject(): Ensures the parent process waits until the child process terminates after reading the message.

## How It Works
The parent process first creates the pipe. It then configures the child process's STARTUPINFO structure to inherit the pipe's read handle as its standard input. After creating the child (cmd.exe /c more), the parent writes a message into the pipe. The child's 'more' command automatically reads this message from its standard input (the pipe) and displays it, proving the IPC was successful.