# Emulator Basics

## What is an Emulator?

An emulator is a software program that recreates the behavior of another hardware system. Instead of executing programs using the original hardware, it simulates each hardware component in software so that programs behave exactly as they would on the physical device.

For this project, the goal was to recreate the Nintendo Game Boy entirely in C++. Every instruction executed by a Game Boy game is interpreted by the emulator, while the hardware components such as memory, graphics, timers, input, and audio are simulated through software.

---

## Why Build an Emulator?

Building an emulator is an excellent way to understand how computers work at a low level. Unlike typical software development, emulator development requires knowledge of:

- Computer architecture
- CPU instruction execution
- Memory organization
- Graphics rendering
- Interrupt handling
- Hardware timing
- Digital audio
- Software design

Rather than relying on operating system APIs to perform these tasks, the emulator recreates the hardware responsible for them.

---

## How Does a Game Execute?

A Game Boy cartridge contains machine code (ROM). When the Game Boy powers on, the CPU begins executing instructions stored inside the cartridge.

Each instruction follows the same basic cycle:

1. Read the next opcode from memory.
2. Decode the opcode.
3. Execute the instruction.
4. Update the program counter.
5. Repeat.

For example:

```
ROM
0x0100 : 3E
0x0101 : 42
```

The CPU reads the opcode `0x3E`, which corresponds to the instruction:

```
LD A, d8
```

The following byte (`0x42`) is loaded into register `A`.

After execution:

```
A = 0x42
PC = 0x0102
```

The CPU immediately continues with the next instruction.

---

## Components of the Emulator

To simulate the Game Boy, the emulator is divided into independent hardware components.

```
GameBoy
│
├── CPU
├── Memory
├── Cartridge
├── PPU
├── Timer
├── Joypad
├── APU
└── Renderer
```

Each component has a dedicated responsibility.

### Cartridge

Loads the ROM into memory and provides data requested by the CPU.

### Memory

Represents the Game Boy's 64 KB address space and routes memory accesses to the appropriate hardware.

### CPU

Executes machine instructions and controls the overall flow of execution.

### PPU

Renders the graphics displayed on the screen.

### Timer

Maintains hardware timers used by games for timing and interrupts.

### Joypad

Processes user input from the keyboard or controller.

### APU

Generates the Game Boy's audio output.

### Renderer

Displays the completed frame using SDL.

---

## How the Components Work Together

Although every subsystem has a different responsibility, they operate together throughout the entire execution of the emulator.

A simplified execution loop looks like this:

```
while (running)
{
    CPU executes an instruction

    Timer advances

    PPU updates graphics

    APU generates audio

    Handle interrupts

    Render completed frame
}
```

Every subsystem depends on CPU cycles to remain synchronized.

---

## Design Philosophy

During development, each hardware component was implemented independently before being integrated into the emulator.

This modular approach provided several advantages:

- Easier debugging
- Better code organization
- Simpler testing
- Clear separation of responsibilities

Instead of creating one large class, every hardware component was encapsulated into its own class.

---

## Key Takeaways

- An emulator recreates hardware using software.
- Each Game Boy subsystem performs a specialized task.
- All hardware components communicate through memory.
- CPU cycle synchronization is essential for correct emulation.
- Modular architecture greatly simplifies development and debugging.