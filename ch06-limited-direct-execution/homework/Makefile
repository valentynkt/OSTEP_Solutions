CC = gcc
CFLAGS = -Wall -g -O0

PROGRAMS = q1_measure_syscall q2_measure_context_switch

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean
