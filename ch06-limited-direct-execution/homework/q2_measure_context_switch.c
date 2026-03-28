// Measure the cost of a context switch using pipe ping-pong
//
// Two processes on two pipes. Each round trip = 2 context switches.
//
//   Parent                     Child
//   write(p1) ------>          read(p1)
//   read(p2)  <------          write(p2)
//
// lldb ./q2_measure_context_switch
//   (lldb) b fork
//   (lldb) run

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define ITERATIONS 100000

int main(void) {
    int p1[2], p2[2];
    char byte = 'x';

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        perror("pipe");
        exit(1);
    }

    int rc = fork();
    if (rc < 0) {
        perror("fork");
        exit(1);
    }

    if (rc == 0) {
        // child: read from p1, write to p2
        for (int i = 0; i < ITERATIONS; i++) {
            read(p1[0], &byte, 1);
            write(p2[1], &byte, 1);
        }
        exit(0);
    }

    // parent: write to p1, read from p2
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < ITERATIONS; i++) {
        write(p1[1], &byte, 1);
        read(p2[0], &byte, 1);
    }
    gettimeofday(&end, NULL);

    long total_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    // each iteration = 1 round trip = 2 context switches
    double avg_us = (double)total_us / (ITERATIONS * 2);

    printf("iterations: %d (%d context switches)\n", ITERATIONS, ITERATIONS * 2);
    printf("total: %ld us\n", total_us);
    printf("avg context switch cost: %.4f us\n", avg_us);
    printf("(includes pipe read/write overhead)\n");

    wait(NULL);
    return 0;
}
