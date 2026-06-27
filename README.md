# GameBruh

*A Nintendo Game Boy (DMG) emulator written from scratch in modern C++ using SDL3 and CMake.*

## Overview

GameBruh is an open-source Nintendo Game Boy emulator developed entirely from scratch as a personal systems programming project. The objective of this project is to accurately emulate the original Game Boy hardware while gaining a deeper understanding of computer architecture, low-level programming, and emulator design.

The emulator implements the major hardware components of the original Game Boy, including the CPU, memory subsystem, graphics processor (PPU), timers, interrupts, DMA, and input handling. Every subsystem has been implemented manually based on publicly available hardware documentation without relying on existing emulator implementations.

In addition to emulation, this repository serves as a learning resource. The accompanying documentation explains the architecture of the Game Boy, the design decisions behind the emulator, and the lessons learned throughout development.

---

## Features

### Implemented

* CPU instruction emulation
* CB-prefixed instruction support
* Accurate CPU flag handling
* Complete Game Boy memory map
* Background rendering
* Window rendering
* Sprite rendering
* Timer emulation
* Interrupt handling
* DMA transfers
* Joypad input handling
* ROM-only cartridge support
* Compatibility with commercial ROM-only games
* Successfully passes all Blargg CPU test ROMs

### Planned

* Audio Processing Unit (APU)
* Additional compatibility improvements
* MBC1 support
* MBC2 support
* MBC3 support
* MBC5 support
* Battery-backed save files
* Save states
* Debugging tools
* Link cable emulation

---

## Requirements

* C++17 compatible compiler
* CMake 3.20 or later
* SDL3

---

## Building

Clone the repository:

```bash
git clone https://github.com/<username>/GameBruh.git
cd GameBruh
```

Generate the build files:

```bash
cmake -B build
```

Compile the project:

```bash
cmake --build build
```

---

## Running

Launch the generated executable and load a compatible Game Boy ROM.

Only ROM-only cartridges are currently supported. Support for Memory Bank Controllers (MBCs) will be added in future releases.

---

## Default Controls

| Keyboard    | Game Boy |
| ----------- | -------- |
| W           | Up       |
| A           | Left     |
| S           | Down     |
| D           | Right    |
| J           | A        |
| K           | B        |
| Enter       | Start    |
| Right Shift | Select   |
| Escape      | Quit     |

---

## Repository Structure

```text
GameBruh/
├── src/                  Source files
├── include/              Header files
├── docs/                 Project documentation
├── screenshots/          Screenshots and media
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Documentation

The `docs` directory contains detailed documentation covering both the emulator implementation and the concepts learned during development.

| Document           | Description                                                                                                                         |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------------- |
| DEVELOPMENT_LOG.md | Chronological record of the project's development, implementation milestones, challenges, and solutions.                            |
| LESSONS_LEARNED.md | Concepts and techniques learned throughout the development process, intended as a learning resource for future emulator developers. |
| ARCHITECTURE.md    | High-level overview of the emulator architecture and component interactions.                                                        |
| ROADMAP.md         | Planned features and future development goals.                                                                                      |

Additional subsystem documentation will be added as development progresses.

---

## Testing

The emulator has been validated using the following:

* Blargg CPU Test ROMs
* Tetris
* Dr. Mario

Further compatibility testing is ongoing.

---

## Objectives

The primary goals of this project are:

* Accurately emulate the Nintendo Game Boy hardware
* Develop a strong understanding of low-level computer architecture
* Explore emulator development techniques
* Apply modern C++ design principles
* Create comprehensive educational documentation for future learners

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for additional information.
