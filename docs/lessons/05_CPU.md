# CPU (Central Processing Unit)

## Introduction

The CPU (Central Processing Unit) is the component responsible for executing every instruction of a Game Boy game. It is often referred to as the "brain" of the system because every operation, from displaying graphics to responding to button presses, is initiated through CPU instructions.

Although other hardware components such as the PPU, Timer, and APU perform specialized tasks, they only operate correctly because the CPU continuously communicates with them through memory.

Understanding how the CPU works is the foundation of emulator development.

---

## The Game Boy CPU

The Nintendo Game Boy uses a Sharp LR35902 processor, an 8-bit CPU derived from the Intel 8080 and Zilog Z80.

Some important specifications are:

- 8-bit processor
- 16-bit address bus
- Clock speed of approximately 4.19 MHz
- Little-endian architecture
- Supports 256 primary opcodes and 256 CB-prefixed opcodes

Unlike modern processors, the Game Boy CPU is relatively simple. It does not have features such as pipelining, multiple cores, or cache memory, making it an excellent processor for understanding computer architecture.

---

## What Does the CPU Actually Do?

At its core, the CPU repeatedly performs one simple task:

1. Read the next instruction.
2. Understand what the instruction means.
3. Execute the instruction.
4. Repeat.

This process is known as the **Fetch–Decode–Execute Cycle**.

For example, suppose memory contains the following bytes:

```
0100 : 3E
0101 : 42
```

The CPU first reads `0x3E`.

Looking at the instruction table, it identifies this as:

```
LD A, d8
```

The instruction requires one additional byte, so the CPU reads `0x42` and stores it in register `A`.

The Program Counter is then updated to point to the next instruction.

This cycle repeats millions of times every second.

---

## Registers

Registers are small storage locations located inside the CPU.

Unlike RAM, registers can be accessed almost instantly, making them ideal for temporary data used during instruction execution.

The Game Boy CPU contains the following 8-bit registers:

```
A
B
C
D
E
F
H
L
```

Some of these registers can also be paired together to form 16-bit registers.

| Register Pair | Consists Of |
|---------------|-------------|
| AF | A + F |
| BC | B + C |
| DE | D + E |
| HL | H + L |

Using register pairs allows the CPU to work with 16-bit addresses while still remaining an 8-bit processor.

---

## The Special Registers

Not all registers store ordinary data.

### Program Counter (PC)

The Program Counter stores the address of the next instruction to execute.

After each instruction, the PC usually advances to the next instruction.

Changing the Program Counter manually allows programs to perform:

- Loops
- Function calls
- Conditional branches
- Jumps

Without the Program Counter, a program would only execute instructions sequentially.

---

### Stack Pointer (SP)

The Stack Pointer points to the top of the stack.

The stack is a region of memory used to temporarily store information.

Typical uses include:

- Function calls
- Returning from functions
- Saving register values
- Interrupt handling

The stack operates using the **Last In, First Out (LIFO)** principle.

```
Push

3
2
1

↓

Pop

1
2
3
```

Whenever data is pushed onto the stack, the Stack Pointer changes automatically.

---

## Flags

The register **F** is called the Flag Register.

Instead of storing ordinary numbers, it stores the results of previous operations.

The Game Boy has four flags.

| Flag | Purpose |
|------|---------|
| Zero (Z) | Result equals zero |
| Subtract (N) | Previous operation was subtraction |
| Half Carry (H) | Carry occurred between bit 3 and bit 4 |
| Carry (C) | Carry occurred beyond bit 7 |

These flags allow future instructions to make decisions.

For example,

```
JP Z, address
```

only performs the jump if the Zero Flag is set.

Without flags, conditional execution would not be possible.

---

## Opcodes

Every CPU instruction has a numerical representation called an **opcode**.

For example,

```
3E
```

represents

```
LD A, d8
```

while

```
AF
```

represents

```
XOR A
```

Instead of reading text-based instructions, the CPU only understands these hexadecimal opcode values.

The emulator therefore requires an opcode table that maps each opcode to its corresponding implementation.

---

## CB-Prefixed Instructions

Some instructions require additional functionality beyond the primary opcode table.

When the CPU encounters opcode:

```
CB
```

it understands that the next byte belongs to a completely different instruction table.

These instructions mainly perform:

- Bit testing
- Bit setting
- Bit resetting
- Rotations
- Shifts

This effectively doubles the number of available instructions.

---

## Instruction Cycles

Not every instruction executes in the same amount of time.

Simple instructions may complete in only a few cycles, while memory accesses, function calls, and conditional jumps require more.

The Game Boy relies heavily on these cycle counts because hardware components such as the Timer and PPU synchronize themselves using CPU cycles.

Accurate cycle counting is therefore just as important as executing the instruction correctly.

---

## Why CPU Accuracy Matters

Many games rely on extremely precise CPU behaviour.

Even small inaccuracies can cause problems such as:

- Incorrect game logic
- Broken animations
- Failed interrupts
- Timing issues
- Graphical glitches

For this reason, implementing every instruction correctly is only part of CPU emulation. The timing and side effects of each instruction must also match the original hardware.

---

## Key Takeaways

- The CPU is responsible for executing every instruction in the game.
- Programs execute through the Fetch–Decode–Execute cycle.
- Registers provide fast temporary storage for instruction execution.
- The Program Counter determines which instruction executes next.
- The Stack Pointer manages function calls and interrupts.
- Flags allow instructions to make decisions based on previous operations.
- Opcodes are numerical representations of machine instructions.
- Correct timing is essential for accurate hardware emulation.