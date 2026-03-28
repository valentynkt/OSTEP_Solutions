// Experiment 5: I/O redirection — stdout to a file
// - close(1) then open() — new file gets fd 1
// - exec'd program writes to file without knowing it
//
// This is what the shell does for: wc file.c > output.txt
//
// After running: cat output.txt to see the result

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD: redirect stdout to file, then exec

        // Step 1: close stdout (fd 1)
        close(STDOUT_FILENO);

        // Step 2: open file — gets fd 1 (lowest available)
        int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // Verify: fd should be 1
        // (using stderr since stdout is redirected)
        fprintf(stderr, "child: opened file on fd = %d (should be 1)\n", fd);

        // Step 3: exec — wc inherits fd table, its printf goes to file
        char *args[] = {"wc", "5_redirect_file.c", NULL};
        execvp("wc", args);
        fprintf(stderr, "exec failed\n");
        exit(1);
    } else {
        // PARENT
        wait(NULL);
        printf("parent: child finished. check 'cat output.txt'\n");
    }

    return 0;
}
