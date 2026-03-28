# 📌 Chapter 5: Process API

The Unix process API — `fork()`, `exec()`, `wait()`, and `pipe()` — and why this seemingly weird interface is actually a brilliant design.

---

## 🔬 Experiments

Small programs I wrote to explore each concept in isolation. Best experienced with a debugger.

| #   | File                                                     | What it explores                                                  |
| --- | -------------------------------------------------------- | ----------------------------------------------------------------- |
| 1   | [`1_fork_basics.c`](experiments/1_fork_basics.c)         | `fork()` return values — parent gets child PID, child gets 0      |
| 2   | [`2_fork_vars.c`](experiments/2_fork_vars.c)             | Variables after `fork()` — same address, independent copies       |
| 3   | [`3_exec_replace.c`](experiments/3_exec_replace.c)       | `exec()` alone — current process is gone, no coming back          |
| 4   | [`4_fork_exec.c`](experiments/4_fork_exec.c)             | `fork()` + `exec()` — the pattern every shell uses                |
| 5   | [`5_redirect_file.c`](experiments/5_redirect_file.c)     | I/O redirection — close stdout, open a file, it takes fd 1        |
| 6   | [`6_redirect_socket.c`](experiments/6_redirect_socket.c) | Same fd trick but over the network — stdout to a TCP socket       |
| 7   | [`7_pipe.c`](experiments/7_pipe.c)                       | `pipe()` connecting two processes — how `cat file \| wc -l` works |

## 📝 Homework Solutions

My answers to the end-of-chapter questions.

| #   | File                                                                | Question                                            | Key idea                                               |
| --- | ------------------------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------------ |
| Q1  | [`q1_fork_variable_copy.c`](homework/q1_fork_variable_copy.c)       | What happens to a variable after `fork()`?          | Each process gets its own copy — changes don't cross   |
| Q2  | [`q2_fork_shared_fd.c`](homework/q2_fork_shared_fd.c)               | Can parent and child write to the same fd?          | Yes — fd table entries are shared across `fork()`      |
| Q3  | [`q3_fork_order_with_pipe.c`](homework/q3_fork_order_with_pipe.c)   | Make child always print before parent (no `wait()`) | Use a pipe as a synchronization primitive              |
| Q5  | [`q5_waitpid_nohang.c`](homework/q5_waitpid_nohang.c)               | What does `waitpid()` with `WNOHANG` return?        | Returns 0 if child still running — non-blocking check  |
| Q8  | [`q8_pipe_between_children.c`](homework/q8_pipe_between_children.c) | Connect two children via a pipe                     | Parent creates pipe, children inherit and redirect fds |

## 🛠️ Build & Run

```bash
# Build everything
cd experiments && make
cd homework && make

# Run an experiment
./experiments/1_fork_basics

# Debug — all binaries compile with -O0 -g
lldb ./experiments/4_fork_exec
(lldb) b fork
(lldb) run
```
