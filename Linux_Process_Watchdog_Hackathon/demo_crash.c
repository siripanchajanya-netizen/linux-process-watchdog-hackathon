/* A harmless demo process that exits after a short time. */
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Demo process is running (PID %d)...\n", (int)getpid());
    fflush(stdout);
    sleep(3);
    printf("Demo process is exiting now.\n");
    return 0;
}
