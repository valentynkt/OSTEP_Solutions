// Measure the cost of a system call (0-byte read on /dev/null)
//
// lldb ./q1_measure_syscall
//   (lldb) b main
//   (lldb) run

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define ITERATIONS 1000000

int main(void) {
    struct timeval start, end;
    char buf;

    // timer precision check
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    gettimeofday(&t2, NULL);
    long precision_us = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
    printf("timer precision: ~%ld us\n", precision_us);

    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    gettimeofday(&start, NULL);
    for (int i = 0; i < ITERATIONS; i++) {
        read(fd, &buf, 0);
    }
    gettimeofday(&end, NULL);

    close(fd);

    long total_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    double avg_us = (double)total_us / ITERATIONS;

    printf("iterations: %d\n", ITERATIONS);
    printf("total: %ld us\n", total_us);
    printf("avg syscall cost: %.4f us\n", avg_us);

    return 0;
}
