# Development Log

## Phase 1 – Project Setup
- Initialized the project using C++ and SDL3.
- Configured the build system with CMake.
- Established the project structure and module organization.

---

## Phase 2 – Cartridge & Memory
- Implemented ROM loading.
- Parsed cartridge headers.
- Implemented the Game Boy memory map.
- Added memory read/write operations.
- Initialized hardware registers.

---

## Phase 3 – CPU Core
- Implemented CPU registers and flag handling.
- Developed the fetch-decode-execute cycle.
- Added stack operations and program counter management.
- Implemented the complete instruction set, including CB-prefixed opcodes.

---

## Phase 4 – Timer & Interrupts
- Implemented DIV and TIMA timers.
- Added timer overflow behavior.
- Implemented interrupt requests and interrupt servicing.
- Integrated interrupt handling with CPU execution.

---

## Phase 5 – Graphics (PPU)
- Implemented LCD timing and PPU modes.
- Added background rendering.
- Implemented window rendering.
- Added sprite rendering.
- Generated complete frame output through the renderer.

---

## Phase 6 – Joypad Input
- Implemented keyboard input mapping.
- Reworked the joypad handling logic.
- Fixed input selection and button state handling.
- Verified functionality using commercial ROMs.

---

## Phase 7 – Audio
- Implemented the Audio Processing Unit (APU).
- Added real-time audio generation.
- Integrated audio with the emulator timing system.

---

## Phase 8 – Testing & Debugging
- Tested the emulator using multiple commercial Game Boy ROMs.
- Resolved CPU execution issues.
- Fixed rendering synchronization problems.
- Corrected timer and interrupt behavior.
- Eliminated input handling bugs.
- Stabilized subsystem integration.

---

## Current Status
- ROM Loading
- Memory Management
- CPU Instruction Set
- Timer
- Interrupts
- Graphics (Background, Window, Sprites)
- Joypad Input
- Audio
- Stable execution of commercial Game Boy ROMs
- Improved ROM compatibility

---

## Planned Features
- ROM selection menu
- Pause and resume
- Save states
- Screenshot support
- Display options
- Controller support
- Performance optimizations
- Code cleanup and documentation