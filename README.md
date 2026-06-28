# Game Boy Emulator

A Nintendo Game Boy emulator written in **C++** using **SDL3**, developed to gain a deeper understanding of computer architecture, hardware emulation, graphics rendering, memory management, and low-level systems programming.

The emulator recreates the core hardware of the original Nintendo Game Boy, including the CPU, memory subsystem, graphics processor (PPU), timers, interrupts, audio, input handling, and multiple Memory Bank Controllers (MBCs).

---

## Features

### CPU

* Complete Game Boy CPU instruction set
* CB-prefixed instructions
* Register and flag emulation
* Stack operations
* Control flow instructions

### Memory

* Full 64 KB memory map
* Memory-mapped I/O
* DMA transfers
* Hardware register initialization

### Graphics (PPU)

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
*  Audio Processing Unit (APU)
* Scanline-based rendering
* LCD timing
* VBlank and LCD interrupts
* Scanline-based rendering
* LCD timing
* VBlank and LCD interrupts


### Timers & Interrupts

* DIV and TIMA timers
* Interrupt handling
* Cycle-based synchronization

### Input

* Joypad emulation
* Keyboard controls

### Audio

* Audio Processing Unit (APU)
* Real-time sound output

### Cartridge Support

* ROM Only
* MBC1
* MBC2
* MBC3
* MBC5

---

## Controls

| Keyboard    | Game Boy Button |
| ----------- | --------------- |
| W           | Up              |
| A           | Left            |
| S           | Down            |
| D           | Right           |
| J           | A               |
| K           | B               |
| Enter       | Start           |
| Right Shift | Select          |

---

## Project Structure

```text
GameBoyEmulator/
│
├── src/
├── include/
├── docs/
│   ├── DEVELOPMENT_LOG.md
│   └── lessons/
├── ROM/
├── assets/
├── CMakeLists.txt
└── README.md
```

---

## Building the Project

### Requirements

* C++17 compatible compiler
* CMake (3.15 or later)
* SDL3

### Clone the Repository

```bash
git clone https://github.com/CassideousxD/GameBruh.git
cd GameBruh
```

### Build

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## Running the Emulator

From the build directory:

```bash
./GameBruh
```

or

```bash
./bin/GameBruh
```

(depending on your build configuration)

---

## Loading ROMs

Place Game Boy ROM files (`.gb`) inside the `ROM/` directory.

Example:

```text
ROM/
├── Tetris.gb
├── DrMario.gb
├── PokemonRed.gb
└── Zelda.gb
```

Launch the emulator and load the desired ROM.

---

## Documentation

Detailed documentation is available in the `docs` directory.

| File               | Description                                         |
| ------------------ | --------------------------------------------------- |
| DEVELOPMENT_LOG.md | Development milestones and progress                 |
| lessons/           | Detailed explanations of Game Boy hardware concepts |

---

## Learning Objectives

This project was developed to better understand:

* Computer Architecture
* Hardware Emulation
* CPU Design
* Memory Management
* Interrupt Handling
* Graphics Rendering
* Digital Audio
* Object-Oriented Design
* Low-Level Systems Programming

---

## Future Improvements

* Save states
* Pause and resume
* ROM selection menu
* Screenshot support
* Controller support
* Display customization
* Performance optimizations
* Expanded ROM compatibility

---

## References

* Pan Docs
* Game Boy CPU Manual
* Blargg Test ROMs
* Mooneye Test Suite

---

## License

This project is licensed under the MIT License.
