
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // CHILD
    printf("child\n");
  } else {
    printf("parent before wait\n");
    int status;
    int child_pid = waitpid(rc, &status, WNOHANG);
    printf("parent\n");
    if (child_pid == -1) {
      perror("wait for child error \n");
      exit(1);
    }
    printf("child_pid: %d \n", child_pid);
    // PARENT
  }
  return 0;
}
