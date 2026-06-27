# Joypad

## Introduction

The Joypad is the primary input device of the Nintendo Game Boy. It allows the player to interact with the game using eight buttons:

- A
- B
- Start
- Select
- Up
- Down
- Left
- Right

Unlike modern computers, where applications receive keyboard or controller events directly from the operating system, the Game Boy CPU communicates with the Joypad through a dedicated hardware register.

The CPU never reads button states directly. Instead, it queries the Joypad hardware whenever it needs to determine the current state of the controller.

---

# The Eight Buttons

The Game Boy controller is divided into two groups.

### Direction Buttons

- Up
- Down
- Left
- Right

### Action Buttons

- A
- B
- Start
- Select

Separating the buttons into two groups reduces the number of hardware lines required to communicate with the controller.

---

# Memory-Mapped Input

The Joypad is accessed through the memory location:

```
FF00
```

This register acts as the communication interface between the CPU and the controller hardware.

Instead of reading every button individually, the CPU reads the Joypad register to determine which buttons are currently pressed.

Because the Joypad is memory-mapped, reading `FF00` behaves differently from reading ordinary RAM.

The returned value depends on the current state of the controller.

---

# Why Are the Buttons Divided into Two Groups?

At first glance, it might seem unnecessary to separate the buttons into directional and action groups.

However, this is a hardware optimization.

The Game Boy CPU has only a limited number of input lines available for communication. Reading all eight buttons simultaneously would require additional hardware.

Instead, the CPU first chooses which group it wants to read.

```
CPU

↓

Select Button Group

↓

Read FF00

↓

Receive Four Button States
```

This technique is known as **multiplexing**, where multiple inputs share the same communication lines.

---

# Active-Low Logic

One characteristic of the Joypad hardware is that it uses **active-low logic**.

Instead of:

```
1 = Pressed

0 = Released
```

the Game Boy uses:

```
0 = Pressed

1 = Released
```

Although this may seem unusual, active-low circuits are common in digital electronics because they simplify hardware design and improve signal reliability.

When implementing an emulator, remembering this inverted logic is essential.

---

# Reading Button States

When the CPU reads the Joypad register, the following sequence occurs:

1. Select the desired button group.
2. Read the register.
3. Determine which buttons are currently pressed.
4. Continue program execution.

Games perform this operation frequently to detect player input.

Because the CPU reads the current state instead of waiting for events, games can respond immediately to button presses.

---

# Joypad Interrupt

In addition to allowing the CPU to read button states, the Joypad can also generate an interrupt.

Whenever a button transitions from released to pressed, the Joypad requests a **Joypad Interrupt**.

This allows the CPU to react quickly without continuously checking the controller.

Interrupt-driven input is particularly useful when the CPU is waiting or performing other tasks.

---

# Input Processing

Although modern computers may receive input from keyboards, game controllers, or other devices, the emulator ultimately converts all of these inputs into the same eight Game Boy buttons.

For example:

```
Keyboard

↓

Game Controller

↓

Emulator

↓

Game Boy Buttons

↓

Joypad Register
```

From the perspective of the game, there is no difference between physical Game Boy buttons and the input device used by the emulator.

---

# Relationship with the CPU

The Joypad does not directly control the game.

Instead, the CPU periodically reads the Joypad register or responds to a Joypad Interrupt.

Based on the returned button states, the game decides how to react.

For example:

- Move the player
- Jump
- Open a menu
- Pause the game
- Fire a projectile

The Joypad simply reports input; the game determines its meaning.

---

# Key Takeaways

- The Game Boy controller consists of eight buttons divided into directional and action groups.
- Input is accessed through the memory-mapped Joypad register (`FF00`).
- The CPU selects which button group to read before obtaining the current button states.
- The Joypad uses active-low logic, where a pressed button is represented by `0`.
- Button presses can also generate Joypad interrupts, allowing the CPU to respond efficiently.
- The emulator translates keyboard or controller input into Game Boy button states before presenting them to the CPU.