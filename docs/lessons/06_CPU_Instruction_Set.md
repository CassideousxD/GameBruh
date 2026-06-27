# CPU Instruction Set

## Introduction

The CPU understands only machine instructions represented by hexadecimal values called **opcodes**. Every opcode tells the processor to perform a specific operation, such as moving data, performing arithmetic, comparing values, or changing the flow of execution.

The Game Boy CPU contains two instruction tables:

- **Primary Opcode Table** (256 instructions)
- **CB-Prefixed Opcode Table** (256 additional instructions)

Although there are over 500 possible opcodes, most belong to a small number of instruction categories. Understanding these categories is more important than memorizing every opcode.

---

# Load Instructions

Load instructions transfer data from one location to another.

The source and destination may be:

- Register to Register
- Immediate Value to Register
- Register to Memory
- Memory to Register
- 16-bit Register Loads

Examples:

```
LD A, B
LD B, C
LD A, (HL)
LD (HL), A
LD A, d8
LD BC, d16
```

Despite the name, **LD does not always mean copying between registers**. It simply means moving data from one location to another.

Load instructions are among the most frequently executed instructions because almost every operation requires data to be moved before it can be processed.

---

# Arithmetic Instructions

Arithmetic instructions perform mathematical operations.

Common operations include:

- Addition
- Subtraction
- Increment
- Decrement
- Addition with Carry
- Subtraction with Borrow

Examples:

```
ADD A, B
ADC A, C
SUB D
SBC A, E
INC H
DEC L
```

Unlike normal calculators, arithmetic instructions also update the Flag Register.

For example,

```
INC B
```

may set the Zero Flag if the result becomes zero.

Similarly,

```
ADD
```

may update the Carry Flag if the result exceeds 8 bits.

Therefore, arithmetic instructions affect both data and processor state.

---

# Logical Instructions

Logical instructions manipulate bits rather than numerical values.

Common operations include:

- AND
- OR
- XOR
- Compare (CP)

Examples:

```
AND B
OR C
XOR A
CP D
```

These instructions are heavily used for:

- Masking bits
- Checking values
- Comparing numbers
- Clearing registers

One important instruction is:

```
CP
```

Although it behaves like subtraction internally, it **does not change the register value**.

Instead, it updates only the flags.

This allows programs to compare two values before making decisions.

---

# Jump Instructions

Normally, the CPU executes instructions one after another.

Jump instructions change this sequence.

Examples:

```
JP address
JR offset
JP Z, address
JR NZ, offset
```

These instructions modify the Program Counter directly.

Without jumps, loops and conditional execution would be impossible.

---

# Call and Return Instructions

Large programs are divided into smaller reusable functions.

The Game Boy achieves this using:

```
CALL
RET
```

When a CALL instruction executes:

1. The address of the next instruction is pushed onto the stack.
2. The Program Counter changes to the function's address.

When RET executes:

1. The return address is popped from the stack.
2. Execution continues from where the function was called.

This mechanism allows code reuse throughout the program.

---

# Stack Instructions

The stack provides temporary storage for the CPU.

Common instructions include:

```
PUSH BC
PUSH DE
POP HL
POP AF
```

These instructions save and restore register values.

The stack is especially important during:

- Function calls
- Interrupt handling
- Temporary data storage

---

# Rotate and Shift Instructions

Sometimes data must be shifted or rotated at the bit level.

Examples include:

```
RLC
RRC
RL
RR
SLA
SRA
SRL
```

These instructions move bits either left or right.

Typical uses include:

- Multiplication by powers of two
- Division by powers of two
- Bit manipulation
- Graphics operations

Most rotate and shift instructions belong to the CB-prefixed opcode table.

---

# Bit Instructions

The Game Boy CPU can manipulate individual bits without affecting the remaining bits.

Three important instruction groups are:

```
BIT
SET
RES
```

### BIT

Checks whether a specific bit is set.

Example:

```
BIT 7, A
```

Only the flags are updated.

---

### SET

Turns a bit on.

Example:

```
SET 3, H
```

---

### RES

Turns a bit off.

Example:

```
RES 5, L
```

Bit instructions are frequently used when working with hardware registers because many registers store multiple settings inside a single byte.

---

# Miscellaneous Instructions

Some instructions do not belong to a major category.

Examples include:

```
NOP
HALT
STOP
DI
EI
DAA
SCF
CCF
CPL
```

Although fewer in number, these instructions control important CPU behavior such as:

- Interrupt enabling
- Interrupt disabling
- Halting execution
- Decimal arithmetic
- Flag manipulation

---

# CB-Prefixed Instructions

The opcode:

```
CB
```

acts as a prefix.

Instead of executing immediately, it tells the CPU that the following byte belongs to the secondary instruction table.

This second table contains instructions for:

- Rotations
- Shifts
- Bit testing
- Bit setting
- Bit resetting

Without this second table, the Game Boy CPU would not have enough opcode space for all required operations.

---

# Why Instruction Categories Matter

At first glance, implementing over 500 instructions appears overwhelming.

However, most instructions follow similar patterns.

For example,

```
INC B
INC C
INC D
INC E
INC H
INC L
```

all perform the same operation on different registers.

Similarly,

```
LD A,B
LD A,C
LD A,D
LD A,E
```

only differ in their source register.

Recognizing these patterns makes implementation significantly easier and reduces duplicate logic.

---

# Key Takeaways

- Every CPU instruction belongs to a small number of categories.
- Load instructions move data between registers, memory, and immediate values.
- Arithmetic and logical instructions update both data and processor flags.
- Jump and Call instructions change program execution flow.
- Stack instructions support function calls and interrupts.
- Rotate, Shift, and Bit instructions manipulate individual bits efficiently.
- Understanding instruction categories is far more valuable than memorizing hundreds of individual opcodes.