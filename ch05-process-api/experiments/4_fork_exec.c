// Experiment 4: fork() + exec() — the standard pattern
// - Fork creates a copy
// - Child calls exec to become a different program
// - Parent survives and waits
//
// Try: set breakpoint on execvp, step into it,
//      notice PID stays the same but code changes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("shell (pid %d): about to run 'wc' on this file\n", getpid());

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD — will become 'wc'
        printf("child (pid %d): about to exec into 'wc'\n", getpid());
        char *args[] = {"wc", "4_fork_exec.c", NULL};
        execvp("wc", args);
        // never reached
        fprintf(stderr, "exec failed\n");
        exit(1);
    } else {
        // PARENT — waits for child
        int status;
        int finished_pid = wait(&status);
        printf("shell (pid %d): child %d finished with status %d\n",
               getpid(), finished_pid, WEXITSTATUS(status));
    }

    return 0;
}
