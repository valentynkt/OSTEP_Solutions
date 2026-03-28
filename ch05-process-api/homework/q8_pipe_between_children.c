
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {
  int pipefd[2];
  pipe(pipefd);
  // --- CHILD 1: writer ---
  int rc1 = fork();
  if (rc1 < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (rc1 == 0) {
    printf("rc1 child pid: %d \n", getpid());
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
    printf("rc1 made write\n");
    fflush(stdout);  // flush before _exit — otherwise pipe gets nothing
    _exit(0);
  }

  // --- CHILD 2: reader ---
  // only parent reaches this point
  int rc2 = fork();
  if (rc2 < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  if (rc2 == 0) {
    printf("rc2 child pid: %d \n", getpid());
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
    printf("rc2 waiting for read from rc1\n");
    char read_buff[100];
    int read_n = read(0, read_buff, sizeof(read_buff) - 1);
    read_buff[read_n] = '\0';
    printf("rc2 received: %s", read_buff);
    fflush(stdout);
    _exit(0);
  }

  // --- PARENT ---
  close(pipefd[0]);  // parent doesn't use pipe — close both ends
  close(pipefd[1]);  // without this, child2's read() hangs forever
  int status_rc1;
  int wait_rc1 = waitpid(rc1, &status_rc1, 0);
  printf("wait rc1\n");
  int status_rc2;
  int wait_rc2 = waitpid(rc2, &status_rc2, 0);
  printf("wait rc2\n");
}
