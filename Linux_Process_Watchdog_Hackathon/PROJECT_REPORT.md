# Project Report: Linux Process Watchdog and Auto-Restart System

## Abstract
The Linux Process Watchdog and Auto-Restart System is a small C-based prototype that monitors a selected program. It waits for the child process to terminate, reports its exit status, and launches it again after a short delay. The project demonstrates basic operating-system process management in a controlled environment.

## Problem Statement
Programs can terminate unexpectedly and may require manual restarting. This project demonstrates how a watchdog can detect process termination and automatically restart a selected program to improve availability.

## Objectives
- Launch a user-selected program.
- Detect when the child process exits.
- Display its process ID and exit status.
- Restart the program after a short delay.
- Allow the user to stop monitoring safely.

## System Design
1. The user starts the watchdog with a command.
2. The watchdog calls `fork()` to create a child process.
3. The child calls `execvp()` to run the requested program.
4. The parent calls `waitpid()` and waits for the child to finish.
5. When the child exits, the parent displays the result and restarts it after two seconds.
6. Ctrl+C requests shutdown and sends SIGTERM to the active child.

## Technologies Used
- C
- GCC or Clang
- POSIX process functions: `fork()`, `execvp()`, `waitpid()`, and `kill()`
- Linux terminal; macOS may also support this POSIX demonstration.

## Testing and Validation
| Test | Input / Action | Expected result |
|---|---|---|
| Normal exit | `./watchdog ./demo_crash` | Demo exits and watchdog starts it again |
| Command arguments | `./watchdog sleep 3` | Process exits after about three seconds and restarts |
| Invalid command | `./watchdog not_a_real_command` | Error is displayed; watchdog retries until stopped |
| Stop | Press Ctrl+C | Watchdog exits and attempts to stop its active child |

## Expected Results
The terminal shows process IDs, exit information, restart attempts, and a final shutdown message. The demonstration illustrates automatic recovery from a process exit.

## Impact
A watchdog can reduce manual intervention for selected applications. The prototype provides a foundation for adding restart limits, logging, notifications, and support for multiple processes.

## Limitations and Future Work
The current prototype monitors one command at a time and restarts indefinitely until stopped. Future versions can add maximum restart counts, increasing retry delays, log files, configuration files, and monitoring of multiple services.

## Responsible Implementation
The project should be tested only with harmless programs created by the team or programs the team is authorized to run. Do not monitor or restart critical system services during the hackathon.
