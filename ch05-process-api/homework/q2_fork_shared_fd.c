#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void) {
  printf("before fork: my pid = %d\n", getpid());
  int state = 1;
  int open_fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  assert(open_fd);
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // CHILD
    char *hello_child = "hello child\n";
    write(open_fd, hello_child, strlen(hello_child));
    state += 1;
    printf("state: %d | state address: %p\n child:  my pid = %d, fork returned "
           "= %d\n",
           state, &state, getpid(), rc);
    close(open_fd);
  } else {
    // PARENT
    char *hello_parent = "hello parent\n";
    write(open_fd, hello_parent, strlen(hello_parent));
    state += 1;
    printf("state: %d | state address: %p\n parent: my pid = %d, fork returned "
           "= %d (child's pid)\n",
           state, &state, getpid(), rc);
    close(open_fd);
  }
  // Both processes run this line
  printf("state: %d | state address: %p \n pid %d exiting\n", state, &state,
         getpid());
  return 0;
}
