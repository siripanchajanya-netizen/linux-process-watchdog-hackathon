/*
 * Linux Process Watchdog and Auto-Restart System
 * Build: gcc watchdog.c -o watchdog
 * Run:   ./watchdog <command> [arguments...]
 *
 * Example safe demos:
 *   ./watchdog sleep 3
 *   ./watchdog ./demo_crash
 *
 * Ctrl+C stops the watchdog and its current child.
 * Designed for Linux/macOS POSIX environments.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

static volatile sig_atomic_t keep_running = 1;
static volatile sig_atomic_t child_pid = -1;

static void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
    if (child_pid > 0) {
        kill((pid_t)child_pid, SIGTERM);
    }
}

int main(int argc, char *argv[]) {
    int restart_count = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        fprintf(stderr, "Example: %s sleep 3\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    printf("Watchdog is monitoring: %s\n", argv[1]);
    printf("Press Ctrl+C to stop.\n");

    while (keep_running) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            sleep(2);
            continue;
        }

        if (pid == 0) {
            execvp(argv[1], &argv[1]);
            perror("execvp");
            _exit(127);
        }

        child_pid = (sig_atomic_t)pid;
        printf("\n[WATCHDOG] Started process PID %d\n", (int)pid);
        fflush(stdout);

        int status = 0;
        pid_t result;
        do {
            result = waitpid(pid, &status, 0);
        } while (result < 0 && errno == EINTR && keep_running);

        child_pid = -1;

        if (!keep_running) {
            break;
        }

        if (result < 0) {
            perror("waitpid");
        } else if (WIFEXITED(status)) {
            printf("[WATCHDOG] Process exited with code %d\n",
                   WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[WATCHDOG] Process ended from signal %d\n",
                   WTERMSIG(status));
        }

        restart_count++;
        printf("[WATCHDOG] Restart attempt #%d in 2 seconds...\n",
               restart_count);
        sleep(2);
    }

    if (child_pid > 0) {
        kill((pid_t)child_pid, SIGTERM);
        waitpid((pid_t)child_pid, NULL, 0);
    }

    printf("\nWatchdog stopped. Restart attempts: %d\n", restart_count);
    return 0;
}
