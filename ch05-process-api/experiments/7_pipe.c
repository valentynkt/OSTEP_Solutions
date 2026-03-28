// Experiment 7: pipe() — connect two processes
// - Simulates: cat 7_pipe.c | wc
// - Child 1 (cat) writes to pipe
// - Child 2 (wc) reads from pipe
//
// pipe() creates two fds: pipefd[0] = read end, pipefd[1] = write end
//
//   cat → writes to pipefd[1] → kernel buffer → pipefd[0] → wc reads

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }
    // pipefd[0] = read end
    // pipefd[1] = write end

    printf("pipe created: read_fd=%d, write_fd=%d\n", pipefd[0], pipefd[1]);

    // --- CHILD 1: cat (writer) ---
    int rc1 = fork();
    if (rc1 == 0) {
        // Writer: stdout → pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); // don't need read end
        close(pipefd[1]); // original fd no longer needed

        char *args[] = {"cat", "7_pipe.c", NULL};
        execvp("cat", args);
        fprintf(stderr, "exec cat failed\n");
        exit(1);
    }

    // --- CHILD 2: wc (reader) ---
    int rc2 = fork();
    if (rc2 == 0) {
        // Reader: stdin → pipe read end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); // don't need write end
        close(pipefd[0]); // original fd no longer needed

        char *args[] = {"wc", NULL};
        execvp("wc", args);
        fprintf(stderr, "exec wc failed\n");
        exit(1);
    }

    // PARENT: close both pipe ends (children have copies)
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children
    wait(NULL);
    wait(NULL);
    printf("done\n");

    return 0;
}
