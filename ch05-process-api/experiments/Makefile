CC = gcc
CFLAGS = -Wall -g -O0

PROGRAMS = 1_fork_basics 2_fork_vars 3_exec_replace 4_fork_exec 5_redirect_file 6_redirect_socket 7_pipe

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean
