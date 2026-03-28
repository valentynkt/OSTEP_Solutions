# OSTEP Solutions & Experiments

My solutions and hands-on experiments while working through [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/) by Remzi and Andrea Arpaci-Dusseau.

Each chapter directory contains:

- **experiments/** — small programs I wrote to explore concepts from the chapter
- **homework/** — my solutions to the end-of-chapter homework questions

## Chapters

| Directory                             | Chapter | Topic                                        |
| ------------------------------------- | ------- | -------------------------------------------- |
| [ch05-process-api](ch05-process-api/) | 5       | Process API — fork(), exec(), wait(), pipe() |

## Building

Each directory has its own `Makefile`:

```bash
cd ch05-process-api/experiments
make        # build all
make clean  # remove binaries
```

## Reference

The OSTEP book's homework simulations and tools are at [ostep-homework](https://github.com/remzi-arpaci-dusseau/ostep-homework).
