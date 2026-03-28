// Experiment 2: fork() copies memory
// - Parent and child get INDEPENDENT copies of variables
// - Modify x in both and see they don't affect each other
//
// Questions to answer:
//   1. What is x in the child right after fork?
//   2. After both modify x, do they see each other's changes?

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int x = 100;
    printf("before fork: x = %d, &x = %p\n", x, (void *)&x);

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD
        printf("child:  x = %d, &x = %p\n", x, (void *)&x);
        x = 200;
        printf("child:  changed x to %d\n", x);
    } else {
        // PARENT
        wait(NULL); // wait for child to finish first
        printf("parent: x = %d, &x = %p\n", x, (void *)&x);
        x = 300;
        printf("parent: changed x to %d\n", x);
    }

    // NOTE: addresses may look the same due to virtual memory,
    // but they are in DIFFERENT physical memory (different processes)

    return 0;
}
