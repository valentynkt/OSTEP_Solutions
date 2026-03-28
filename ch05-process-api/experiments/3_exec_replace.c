// Experiment 3: exec() WITHOUT fork — replaces current process
// - After exec, this program is GONE
// - The printf after exec never runs
//
// Run this and notice: you get ls output, but never see "after exec"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("before exec: my pid = %d\n", getpid());
    printf("about to become 'ls'...\n");

    char *args[] = {"ls", "-la", NULL};
    execvp("ls", args);

    // This NEVER executes (unless exec fails)
    printf("ERROR: if you see this, exec failed\n");
    return 1;
}
