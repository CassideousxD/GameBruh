# Timers

## Introduction

Computers need a reliable way to measure time. Many operations depend on precise timing, such as animations, player movement, sound generation, and event scheduling.

Unlike humans, a CPU has no understanding of seconds or milliseconds. It only executes instructions one after another. Therefore, a hardware mechanism is required to convert CPU execution into measurable time.

The Game Boy achieves this using hardware timers.

---

# Why Are Timers Needed?

Imagine a game where an enemy moves every second.

Without a timer, the CPU would have to repeatedly count instructions or continuously check whether enough time had passed. This approach would be inaccurate because different instructions require different amounts of time to execute.

A dedicated timer provides a consistent way to measure elapsed time regardless of what the CPU is doing.

Games use timers for tasks such as:

- Character movement
- Animations
- Sound playback
- Event scheduling
- Physics updates
- Interrupt generation

---

# CPU Cycles and Time

The Game Boy CPU operates at approximately **4.19 MHz**, meaning it performs about 4.19 million clock cycles every second.

Instead of measuring time in seconds, the hardware measures time using **CPU cycles**.

Every instruction consumes a certain number of cycles.

For example:

| Instruction | Cycles |
|------------|-------:|
| NOP | 4 |
| LD A, B | 4 |
| LD A, (HL) | 8 |
| CALL | 24 |

Because every instruction has a known cycle count, hardware components can determine how much time has passed simply by counting cycles.

This cycle-based timing keeps every subsystem synchronized.

---

# The Game Boy Timer Registers

The Game Boy uses several memory-mapped registers to control its timers.

| Register | Address | Purpose |
|----------|----------|---------|
| DIV | FF04 | Divider Register |
| TIMA | FF05 | Timer Counter |
| TMA | FF06 | Timer Modulo |
| TAC | FF07 | Timer Control |

Each register has a different responsibility.

---

# DIV (Divider Register)

The Divider Register is a continuously incrementing counter.

Unlike other timers, it begins counting as soon as the Game Boy is powered on and continues running regardless of the program being executed.

Its purpose is to provide a continuously changing value that software can use for timing-related tasks.

An important characteristic of the DIV register is that writing any value to it resets the counter back to zero.

The value being written is ignored.

This behavior is unique and differs from most other memory locations.

---

# TIMA (Timer Counter)

TIMA is the primary programmable timer used by games.

Unlike DIV, TIMA does not always run.

Its behavior depends on the Timer Control register (TAC).

When enabled, TIMA increments at a selectable frequency.

Once TIMA reaches its maximum value (`0xFF`) and increments again, an overflow occurs.

Instead of continuing to `0x100`, two important actions happen:

1. TIMA is reloaded using the value stored in TMA.
2. A Timer Interrupt is requested.

This allows games to perform actions at regular intervals without constantly checking elapsed time.

---

# TMA (Timer Modulo)

TMA stores the value that TIMA should reload after an overflow.

For example,

```
TMA = 80
```

If TIMA overflows,

```
FF

↓

Overflow

↓

Reload

↓

80
```

Using TMA allows games to generate periodic timer events without restarting the counter from zero each time.

---

# TAC (Timer Control)

The Timer Control register determines whether TIMA is active and how quickly it increments.

It controls two things:

1. Whether the timer is enabled.
2. Which clock frequency is used.

Different frequencies allow software to generate interrupts at different time intervals depending on its needs.

---

# Timer Overflow

The most important event in timer operation is an overflow.

Suppose TIMA currently contains:

```
FE
```

After one increment:

```
FF
```

After another increment:

```
Overflow
```

Instead of continuing beyond `FF`:

- TIMA is reloaded from TMA.
- A Timer Interrupt is requested.

This interrupt allows the CPU to execute code associated with timed events.

---

# Relationship Between Timers and Interrupts

Timers and interrupts work closely together.

The timer simply counts cycles.

When an overflow occurs, it does not directly execute code.

Instead, it requests a Timer Interrupt.

The CPU then decides when it is safe to temporarily pause the current program and execute the timer interrupt routine.

This separation allows the timer hardware to remain simple while giving the CPU full control over interrupt handling.

---

# Why Cycle Accuracy Matters

Timers do not measure real-world time directly.

Instead, they measure CPU cycles.

If an emulator incorrectly counts instruction cycles:

- Animations may run too quickly or too slowly.
- Sound timing may become inaccurate.
- Games may execute events earlier or later than expected.
- Interrupts may occur at incorrect times.

For this reason, accurate cycle counting is one of the most important aspects of emulator development.

---

# Key Takeaways

- Timers allow the Game Boy to measure the passage of time using CPU cycles.
- Every instruction consumes a fixed number of cycles, which hardware components use for synchronization.
- DIV is a continuously running divider register.
- TIMA is the programmable timer used by games.
- TMA stores the reload value after a TIMA overflow.
- TAC controls whether the timer is enabled and its operating frequency.
- Timer overflows generate interrupts, allowing software to perform periodic tasks.
- Correct timer behavior depends on accurate CPU cycle counting.