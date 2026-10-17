# Linux Process Watchdog and Auto-Restart System

## 1. Problem statement
A process may terminate unexpectedly, interrupting a task or service. This project demonstrates a small watchdog that waits for a selected program to exit and then starts it again after a short delay.

## 2. Features
- Starts a selected command with optional arguments.
- Detects process termination using `waitpid()`.
- Restarts the command after a two-second delay.
- Displays the process ID, exit code/signal, and restart count.
- Stops when the user presses Ctrl+C.
- Includes a harmless demo program for presentation and testing.

## 3. Technologies
- C programming language
- POSIX process APIs: `fork()`, `execvp()`, `waitpid()`, `kill()`
- GCC or Clang compiler
- Linux terminal (macOS may also work for this POSIX demo)

## 4. Files
- `watchdog.c` — watchdog implementation
- `demo_crash.c` — safe demonstration process that exits after three seconds
- `PROJECT_REPORT.md` — concise project report

## 5. Compile
Open a terminal in this folder and run:

```bash
gcc watchdog.c -o watchdog
gcc demo_crash.c -o demo_crash
```

If `gcc` is unavailable on macOS, try `clang` instead.

## 6. Run the demonstration
Run:

```bash
./watchdog ./demo_crash
```

The demo program runs for about three seconds, exits, and the watchdog restarts it after two seconds. Let it restart two or three times, then press **Ctrl+C**.

You can also test a command with arguments:

```bash
./watchdog sleep 3
```

## 7. Testing checklist
1. Valid command: `./watchdog ./demo_crash` — should repeatedly start the demo.
2. Command with arguments: `./watchdog sleep 3` — should detect each normal exit and restart.
3. Invalid command: `./watchdog not_a_real_command` — should report an execution error and retry; stop it with Ctrl+C.
4. Stop behavior: press Ctrl+C — watchdog should stop and attempt to terminate its current child.

## 8. Safety notes / limitations
- Run only harmless test commands that you own or are authorized to control.
- Do not use this demo to restart critical system processes.
- This is a prototype, not a production service manager.
- It does not yet include restart limits, exponential backoff, daemon mode, persistent logs, or multi-process monitoring.
- A production watchdog should limit retries to prevent crash loops and should carefully manage child processes.

## 9. GitHub upload
Create a new repository on GitHub, for example `linux-process-watchdog`. Upload `watchdog.c`, `demo_crash.c`, `README.md`, and `PROJECT_REPORT.md`. Add the PPT separately if your instructor wants presentation files in the repository.
