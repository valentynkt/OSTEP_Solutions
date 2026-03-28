# Chapter 7: Scheduling — Introduction

The OS must decide which process runs next. Two families of schedulers emerge: one optimizes turnaround (completion speed), the other optimizes response (interactivity). You can't have both.

---

## The Method: Relax Assumptions One by One

Start with 5 unrealistic assumptions, build the simplest scheduler, remove an assumption, discover the problem, build a better scheduler. Repeat.

```
Assumption relaxed:             Scheduler unlocked:
───────────────────             ──────────────────
1. All jobs same length    ───→  SJF   (shortest first)
2. Jobs arrive same time   ───→  STCF  (preempt for shorter arrival)
3. Jobs run to completion  ───→  (enables preemption as mechanism)
4. Jobs do I/O             ───→  Overlap (CPU bursts as sub-jobs)
5. Job length known        ───→  ???   (Ch.8: MLFQ)
```

---

## Two Metrics

### Turnaround Time

**What it measures:** how long YOU waited from submitting a job to getting results.

```
T_turnaround = T_completion - T_arrival
```

A job arriving at t=50 and finishing at t=60 has turnaround 10 — same as one arriving at t=0 finishing at t=10. It's wall-clock time from the submitter's perspective. Average turnaround across jobs = how painful the scheduler is for the typical job.

### Response Time

**What it measures:** how long until the system reacts to your job at all.

```
T_response = T_first_run - T_arrival
```

Critical for interactive use — typing in a shell and waiting 10 seconds for a reaction is unacceptable.

### The Fundamental Tradeoff

```
Optimizes Turnaround    ←── TRADEOFF ──→    Optimizes Response
    SJF / STCF                                    RR
    UNFAIR                                        FAIR
 (short jobs first,                          (everyone gets a turn,
  long jobs wait)                             all jobs finish late)
```

Fairness kills turnaround. Unfairness kills response. No scheduler beats both.

---

## The Schedulers

### FIFO (First In, First Out)

Run jobs in arrival order. Simple, but suffers from the **convoy effect**: one long job blocks everything behind it.

```
Jobs: A=100s, B=10s, C=10s (all arrive at t=0)

FIFO order: A → B → C
Timeline:   [AAAAAAAAAA...100s...][BB][CC]
Turnaround: A=100, B=110, C=120  → avg = 110
```

### SJF (Shortest Job First)

Run shortest job first. Optimal for turnaround _when all jobs arrive at the same time_.

```
Same jobs, SJF order: B → C → A
Timeline:   [BB][CC][AAAAAAAAAA...100s...]
Turnaround: B=10, C=20, A=120  → avg = 50
```

**Limitation:** if jobs arrive at different times, a long job already running blocks short arrivals — same convoy problem returns.

```
A arrives t=0 (100s), B arrives t=10 (10s), C arrives t=10 (10s)

SJF (non-preemptive): [AAAA...100s...][BB][CC]
                       ↑ B,C arrive but must wait
Turnaround: A=100, B=100, C=110  → avg = 103.33
```

### STCF (Shortest Time-to-Completion First)

SJF + preemption. When a new job arrives, compare its total time to the _remaining_ time of the running job. Switch if the new one is shorter.

```
A arrives t=0 (100s), B arrives t=10 (10s), C arrives t=10 (10s)

STCF: [AA..10s..][BB][CC][AAA...remaining 90s...]
       ↑ B,C arrive → preempt A (90s remaining > 10s)

Turnaround: A=120, B=10, C=20  → avg = 50
```

Provably optimal for turnaround. But terrible for response when jobs pile up — the third job waits for two full jobs before it runs once.

### RR (Round Robin)

Run each job for one **time slice**, then switch to the next. Cycle until done.

```
A=5s, B=5s, C=5s (all arrive t=0, time slice = 1s)

RR: [A][B][C][A][B][C][A][B][C]...
Response: A=0, B=1, C=2  → avg = 1
Turnaround: A=13, B=14, C=15  → avg = 14  (awful)
```

---

## Timer Interrupt vs Time Slice

This distinction matters for understanding RR.

```
HARDWARE LAYER              OS LAYER
──────────────              ────────
Timer chip fires       →    OS gets control back
every N ms                  (interrupt handler runs)
       ↑                           ↑
TIMER INTERRUPT PERIOD      OS DECIDES: switch or not?
(fixed, hardware config)    TIME SLICE = policy choice

Time:  0    10    20    30    40    50
       |     |     |     |     |     |
       ↑     ↑     ↑     ↑     ↑     ↑
       Timer interrupts (every 10ms)

Time slice = 10ms → switch at every interrupt
Time slice = 30ms → switch every 3rd interrupt
Time slice = 15ms → IMPOSSIBLE (no interrupt fires at t=15)
```

**Timer interrupt** = mechanism (hardware gives OS control periodically).
**Time slice** = policy (how many interrupt periods before switching).
Time slice must be a multiple of the interrupt period.

---

## Time Slice Tradeoff: Amortization

Shorter time slice = better response time, but more context switch overhead.

```
Time slice  | Context switch | Overhead | Response
────────────┼────────────────┼──────────┼─────────
10 ms       | 1 ms           | 10%      | Great
100 ms      | 1 ms           | 1%       | Worse
1000 ms     | 1 ms           | 0.1%     | Bad
```

Context switch cost is not just saving/restoring registers — it's also flushing CPU caches, TLBs, branch predictors. The new process starts "cold."

**Amortization**: make the time slice large enough that switch cost becomes negligible, but short enough to stay responsive.

---

## Incorporating I/O: Overlap

When a job blocks on I/O, don't waste the CPU — run another job.

```
WITHOUT OVERLAP:
CPU:  [AAAAA][─────wait─────][AAAAA][BBBBB]
Disk: [─────][AAAAA I/O─────][─────][─────]
                                     ↑ B waits for everything

WITH OVERLAP:
CPU:  [AAAAA][BBBBB][AAAAA][BBBBB][AAAAA]
Disk: [─────][AAA──][─────][AAA──][─────]
       ↑ A does I/O while B runs CPU
```

The key insight: treat each CPU burst of an I/O-heavy job as a **separate schedulable unit**. A job doing 10ms CPU + 10ms I/O repeatedly looks like many 10ms sub-jobs to STCF, and naturally gets scheduled between B's longer bursts.

---

## The Unsolved Problem

All of SJF/STCF require knowing job lengths in advance. Real OSes don't have this oracle. Chapter 8 introduces MLFQ: a scheduler that **learns from the recent past to predict the future**, combining turnaround and response time without knowing job lengths.

---

## Retrieval Questions

Answer from memory before checking.

1. What are the two scheduling metrics? Write the formula for each.
2. Why is SJF not optimal when jobs arrive at different times? What fixes it?
3. What is the convoy effect and which scheduler suffers from it?
4. RR is great for response time — why is it terrible for turnaround?
5. Why must the time slice be a multiple of the timer interrupt period?
6. How does the OS overlap I/O with CPU work? What abstraction makes this work with STCF?
7. What's the fundamental tradeoff between turnaround-optimal and response-optimal schedulers?

---

## Gaps I Had (For Honest Review)

- **Turnaround intuition was fuzzy** — described it as "when the average job will complete." Now clear: it's wall-clock time from the submitter's perspective per job. Average across jobs measures scheduler pain.
- **Timer interrupt vs time slice was confused** — knew time slice should be a multiple of interrupt period but didn't understand why. Now clear: interrupt = mechanism (hardware gives OS control), time slice = policy (OS decides how many periods before switching). Can't switch at t=15 if interrupts only fire at t=10, t=20.
- **I/O overlap was surface-level** — described it as "round robin with overlapping." More precise: treat each CPU burst as an independent sub-job for scheduling. STCF naturally picks the shorter burst, and the blocked job's I/O runs in parallel on disk.
