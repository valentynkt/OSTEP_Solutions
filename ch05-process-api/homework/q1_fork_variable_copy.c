#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>
int main(void) {
  printf("before fork: my pid = %d\n", getpid());
  int state = 1;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // CHILD
    state += 1;
    printf("state: %d | state address: %p\n child:  my pid = %d, fork returned "
           "= %d\n",
           state, &state, getpid(), rc);
  } else {
    // PARENT
    state += 1;
    printf("state: %d | state address: %p\n parent: my pid = %d, fork returned "
           "= %d (child's pid)\n",
           state, &state, getpid(), rc);
  }
  // Both processes run this line
  printf("state: %d | state address: %p \n pid %d exiting\n", state, &state,
         getpid());
  return 0;
}
