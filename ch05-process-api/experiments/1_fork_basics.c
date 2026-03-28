// Experiment 1: fork() basics
// - Watch the PID and return values
// - Set breakpoint on fork() and step through
//
// Try: lldb ./1_fork_basics
//   (lldb) b fork
//   (lldb) run
//   (lldb) step over, watch rc value in parent vs child

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    printf("before fork: my pid = %d\n", getpid());

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD
        printf("child:  my pid = %d, fork returned = %d\n", getpid(), rc);
    } else {
        // PARENT
        printf("parent: my pid = %d, fork returned = %d (child's pid)\n", getpid(), rc);
        wait(NULL);
    }

    // Both processes run this line
    printf("pid %d exiting\n", getpid());
    return 0;
}
