CC = gcc
CFLAGS = -Wall -g -O0

PROGRAMS = q1_fork_variable_copy q2_fork_shared_fd q3_fork_order_with_pipe q5_waitpid_nohang q8_pipe_between_children

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS) *.output

.PHONY: all clean
