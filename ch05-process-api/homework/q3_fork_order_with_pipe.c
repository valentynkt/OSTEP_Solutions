#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int pipefd[2];
    pipe(pipefd);

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD
        printf("hello\n");
        write(pipefd[1], "x", 1); // signal parent
        close(pipefd[0]);
        close(pipefd[1]);
    } else {
        // PARENT
        char buf;
        read(pipefd[0], &buf, 1); // blocks until child writes
        printf("goodbye\n");
        close(pipefd[0]);
        close(pipefd[1]);
    }
    return 0;
}
