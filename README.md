# 🖥️ OSTEP — My OS Journey

> In a world full of AI-generated code and vibe coding, there's something deeply satisfying about opening Neovim, writing C, and watching a `fork()` split your process in two.

Hands-on experiments and homework solutions as I work through [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) by Remzi & Andrea Arpaci-Dusseau.

Every line here is written by hand, stepped through with `lldb`, and understood before it was committed.

---

## 📂 Structure

Each chapter follows the same layout:

```
ch<number>-<topic>/
├── experiments/   # exploratory programs I wrote to poke at concepts
├── homework/      # my solutions to the book's questions
├── Makefile       # build everything with `make`
└── README.md      # what I learned + file index
```

## 📖 Chapters

| Status | Chapter                                 | Topic                               | Highlights                          |
| ------ | --------------------------------------- | ----------------------------------- | ----------------------------------- |
| ✅     | [Ch 5 — Process API](ch05-process-api/) | `fork()` `exec()` `wait()` `pipe()` | 7 experiments, 5 homework solutions |
| 🔜     | Ch 7 — CPU Scheduling                   | Scheduling policies                 | —                                   |
| 🔜     | Ch 13 — Address Spaces                  | Virtual memory                      | —                                   |
| 🔜     | Ch 26 — Concurrency                     | Threads                             | —                                   |
| 🔜     | ...                                     |                                     |                                     |

> 💡 This is a living repo — new chapters added as I work through the book.

## 🛠️ Build & Run

```bash
cd ch05-process-api/experiments
make            # compiles all programs (-Wall -g -O0)
./1_fork_basics # run

# debug with lldb
lldb ./1_fork_basics
(lldb) b main
(lldb) run
```

## 🔗 Resources

- 📕 [OSTEP — free online textbook](https://pages.cs.wisc.edu/~remzi/OSTEP/)
- 🧰 [ostep-homework — book's official simulators & tools](https://github.com/remzi-arpaci-dusseau/ostep-homework)

## 🧑‍💻 Tools

- **Editor:** Neovim (LazyVim)
- **Debugger:** LLDB
- **Language:** C99
- **OS:** macOS (ARM64)
