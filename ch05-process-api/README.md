# Chapter 5: Process API

## Experiments

Hands-on explorations of Unix process primitives, meant to be stepped through with a debugger.

| #   | File                  | Concept                                                 |
| --- | --------------------- | ------------------------------------------------------- | ---- |
| 1   | `1_fork_basics.c`     | fork() return values and PIDs                           |
| 2   | `2_fork_vars.c`       | fork() copies memory — independent address spaces       |
| 3   | `3_exec_replace.c`    | exec() without fork — replaces current process          |
| 4   | `4_fork_exec.c`       | fork() + exec() — the standard shell pattern            |
| 5   | `5_redirect_file.c`   | I/O redirection — stdout to a file via close/open       |
| 6   | `6_redirect_socket.c` | stdout to a TCP socket — same fd trick over the network |
| 7   | `7_pipe.c`            | pipe() connecting two processes (simulates `cat         | wc`) |

## Homework

Solutions to the homework questions from OSTEP Chapter 5.

| #   | File                         | Question                                         |
| --- | ---------------------------- | ------------------------------------------------ |
| Q1  | `q1_fork_variable_copy.c`    | What happens to a variable after fork()?         |
| Q2  | `q2_fork_shared_fd.c`        | Can parent and child both write to the same fd?  |
| Q3  | `q3_fork_order_with_pipe.c`  | Ensure child prints before parent without wait() |
| Q5  | `q5_waitpid_nohang.c`        | What does waitpid() with WNOHANG return?         |
| Q8  | `q8_pipe_between_children.c` | Pipe communication between two child processes   |

## Build & Run

```bash
# Experiments
cd experiments && make && ./1_fork_basics

# Homework
cd homework && make && ./q1_fork_variable_copy

# Debug with lldb
make clean && make    # builds with -O0 -g
lldb ./1_fork_basics
```
