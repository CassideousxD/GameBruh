# Project Architecture

## Overview

Before implementing individual hardware components, it was important to decide how the emulator would be structured. Instead of writing all functionality in a single file, the emulator was divided into independent classes that closely resemble the hardware found inside the original Game Boy.

This modular design made the project easier to understand, maintain, test, and debug.

---

## Overall Architecture

The emulator consists of the following major components:

```
                 +----------------+
                 |    GameBoy     |
                 +--------+-------+
                          |
      +-------------------+--------------------+
      |        |        |        |       |      |
      v        v        v        v       v      v
    CPU     Memory   Cartridge  PPU   Timer  Joypad
                                              |
                                              v
                                             APU
```

Each class is responsible for simulating one hardware component.

---

## The GameBoy Class

The `GameBoy` class acts as the central controller of the emulator.

Instead of containing all emulator logic, it owns every hardware component and coordinates communication between them.

Responsibilities:

- Create every subsystem
- Connect the subsystems together
- Load the ROM
- Initialize SDL
- Execute the main emulation loop

A simplified version looks like:

```cpp
class GameBoy
{
private:
    CPU cpu;
    Memory memory;
    Cartridge cartridge;
    PPU ppu;
    Timer timer;
    Joypad joypad;
    APU apu;
    Renderer renderer;
};
```

Keeping everything inside a single class makes ownership clear and prevents unnecessary global variables.

---

## Why Separate Classes?

Initially, it might seem easier to place everything inside one large class.

However, doing so quickly becomes difficult to maintain.

For example,

The CPU should only be responsible for executing instructions.

It should **not** contain:

- Graphics rendering
- Audio generation
- Keyboard input
- SDL rendering

Similarly,

The PPU should only know how to draw graphics.

It should not execute CPU instructions.

Separating responsibilities follows the **Single Responsibility Principle**, making each class easier to develop independently.

---

## Communication Between Components

Although every component has its own responsibility, they constantly communicate with one another.

For example,

When the CPU executes

```cpp
LD (0xC000), A
```

it does not directly modify memory.

Instead, it calls

```cpp
memory.writeByte(address, value);
```

The Memory class then determines what exists at that address.

Depending on the address, the write may affect:

- RAM
- VRAM
- OAM
- Hardware registers
- Joypad
- Timer
- PPU

This makes the Memory class the communication hub of the emulator.

---

## Dependency Relationships

Not every component communicates directly.

The relationships are intentionally kept simple.

```
CPU
 │
 ▼
Memory
 │
 ├── Cartridge
 ├── PPU
 ├── Timer
 ├── Joypad
 └── APU
```

The CPU rarely talks directly to other hardware.

Instead, almost all communication happens through memory reads and writes, just like the real Game Boy.

This closely mirrors the actual hardware architecture.

---

## Why Use References?

Several classes need access to the same Memory object.

Instead of creating copies, references are used.

Example:

```cpp
class CPU
{
private:
    Memory& memory;
};
```

This ensures that every component operates on the exact same memory space.

If each class had its own copy, the emulator would behave incorrectly because hardware components would no longer stay synchronized.

---

## Execution Flow

The emulator starts from `main.cpp`.

```
main()
    │
    ▼
Create GameBoy
    │
    ▼
Load ROM
    │
    ▼
Initialize Hardware
    │
    ▼
Run Emulator
```

Inside the GameBoy execution loop:

```
while (running)
{
    CPU executes instructions

    Timer updates

    PPU updates

    APU updates

    Process user input

    Render completed frame
}
```

This loop continues until the emulator is closed.

---

## Why This Design Worked Well

As the project grew, new components could be added without affecting existing code.

For example,

Adding the Audio Processing Unit only required:

- Creating a new APU class
- Connecting it inside GameBoy
- Updating the main loop

No major changes were required in the CPU or PPU.

This modularity made future development significantly easier.

---

## Lessons Learned

- Divide large systems into small, independent components.
- Each class should have one clearly defined responsibility.
- Shared hardware should be accessed through references rather than duplicated.
- Memory acts as the communication bridge between most Game Boy hardware.
- A modular architecture makes debugging and future feature additions much easier.