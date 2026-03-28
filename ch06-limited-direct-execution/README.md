# Chapter 6: Limited Direct Execution

The OS runs programs at native CPU speed while keeping control. Three hardware mechanisms make this possible.

---

## The Name Decoded

- **Direct** — program runs natively on CPU (not interpreted/emulated)
- **Limited** — hardware enforces what the program can do
- **Execution** — runtime protocol, not a compile-time thing

### Why "direct" matters

The alternative is **interpretation**: a software loop reads each instruction and simulates it. Safe (sees every instruction) but 10-100x slower. Limited Direct Execution gets native speed AND safety by making the hardware the enforcer.

```
INTERPRETATION                    DIRECT EXECUTION
Program → Interpreter → CPU       Program → CPU
(safe, slow)                      (fast, dangerous alone)

LIMITED DIRECT EXECUTION = Direct + hardware boundaries
(fast AND safe)
```

---

## Three Mechanisms

### 1. Dual Mode (user / kernel)

CPU has two modes. User mode restricts the instruction set — no I/O, no interrupt control, no trap table modification. Kernel mode has full access.

### 2. Trap / Return-from-trap

The safe gate between modes. Three actors, strict roles:

| Actor        | Does what                                                                      |
| ------------ | ------------------------------------------------------------------------------ |
| User program | Picks syscall **number** (never a kernel address)                              |
| Hardware     | Saves regs → kernel stack, switches mode, jumps to handler from **trap table** |
| OS           | Does the work, then executes return-from-trap                                  |

**Security property**: the user program never chooses WHERE in the kernel to jump. The trap table (set at boot, privileged to modify) maps numbers to addresses. Without this, a program could jump past permission checks inside kernel functions.

```
User: "syscall #0 (read)"
         │
Hardware: save regs → k-stack
         switch user → kernel
         trap table[0] → 0x1000
         jump to 0x1000
         │
OS:      handle read()
         return-from-trap
         │
Hardware: restore regs ← k-stack
         switch kernel → user
         jump to saved PC
         │
User:    resumes (never knew it left)
```

### 3. Timer Interrupt

What if a process never traps? (infinite loop, no syscalls)

**Cooperative scheduling**: OS waits for voluntary traps. Fatal flaw — infinite loop = reboot.

**Non-cooperative (timer)**: hardware interrupts the CPU every N ms. Same mechanism as trap — save regs, switch to kernel, jump to timer handler. The OS now runs and decides what to do.

Started at boot (privileged). Cannot be disabled from user mode.

---

## Context Switch — The Two-Level Save

When the OS decides to switch from process A to B, two saves happen by two different actors to two different places:

```
┌─────────┬──────────┬───────────────┬─────────────────┐
│  Level  │   Who    │    What       │     Where       │
├─────────┼──────────┼───────────────┼─────────────────┤
│ Save 1  │ Hardware │ User regs     │ Kernel stack    │
│ Save 2  │ OS       │ Kernel regs   │ Process struct  │
├─────────┼──────────┼───────────────┼─────────────────┤
│Restore 2│ OS       │ Kernel regs   │ Process struct  │
│Restore 1│ Hardware │ User regs     │ Kernel stack    │
└─────────┴──────────┴───────────────┴─────────────────┘
```

Save 1 is automatic (happens on ANY trap/interrupt). Save 2 is deliberate (only when the OS decides to switch). The stack pointer swap is the moment identity changes — the OS enters as A, returns as B.

**Per-process kernel stacks**: each process has its own. Without this, process B trapping would overwrite A's saved registers. Not just a security issue — a **correctness** issue.

---

## Two Phases of Operation

**Boot** (once, kernel mode): install trap table, start timer, create first process.

**Run** (repeated cycle):

```
┌──→ Process runs (user mode)
│          │
│    trap or timer interrupt
│          ↓
│    OS runs (kernel mode)
│    maybe context switch
│          │
└─── return-from-trap
```

---

## Edge Case: Interrupt During Interrupt

Timer fires while OS handles a syscall. Shared data structures may be half-modified. Solution: **disable interrupts** during critical sections (privileged instruction). Tradeoff: too long = missed interrupts. Full treatment in the concurrency chapters.

---

## Homework (Measurement)

| File                                                                  | What it measures    | How                                                                       |
| --------------------------------------------------------------------- | ------------------- | ------------------------------------------------------------------------- |
| [`q1_measure_syscall.c`](homework/q1_measure_syscall.c)               | Syscall cost        | 0-byte `read()` on `/dev/null` in a loop, divide total time by iterations |
| [`q2_measure_context_switch.c`](homework/q2_measure_context_switch.c) | Context switch cost | Two processes, two pipes, ping-pong. Each blocked read forces a switch    |

Results on Apple Silicon: ~0.27 us/syscall, ~1.6 us/context switch (upper bound, includes pipe overhead).

```bash
cd homework && make
./q1_measure_syscall
./q2_measure_context_switch
```

---

## Retrieval Questions

Use these for spaced repetition review. Answer from memory before checking.

1. What are the three hardware mechanisms that enable limited direct execution?
2. Who translates a syscall number into a kernel address — user, hardware, or OS?
3. Why can't a user program modify the trap table?
4. What's the difference between cooperative and non-cooperative scheduling? When does cooperative fail?
5. Name the two levels of register saving during a context switch — who does each, and where do the registers go?
6. Why does each process need its own kernel stack?
7. What happens if an interrupt fires while the OS is already handling an interrupt?

---

## Gaps I Had (For Honest Review)

- **"Why does the book say direct execution?"** — didn't know the alternative (interpretation) existed. Now clear: direct = native speed, the alternative is 10-100x slower.
- **Trap mechanism was vague** — said "trap handlers or something." Now precise: trap table set at boot, hardware does the save/switch/jump, user picks a number not an address.
- **Cooperative scheduling was tangled** — mixed up "waiting for I/O" with the concept. It's simpler: OS only runs when process voluntarily traps. Infinite loop = game over.
- **Context switch two-level save was missing** — didn't mention it at all. Key insight: hardware does Save 1 automatically, OS does Save 2 deliberately. Two actors, two destinations.
- **Interrupt during interrupt** — didn't mention. Simple answer: disable interrupts during critical sections.
