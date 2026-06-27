# Cartridge

## Overview

The Game Boy cartridge contains the game itself. It stores the program code (ROM), game data, and metadata such as the game title, cartridge type, ROM size, and RAM size.

When a cartridge is inserted into a Game Boy, the CPU begins executing instructions directly from the cartridge ROM. Therefore, the first step in building the emulator was to load the ROM into memory so that the CPU could access it.

---

## What is a ROM?

ROM stands for **Read-Only Memory**.

Unlike RAM, ROM is not modified while the game is running. It permanently stores the game code that was programmed by the game developer.

For example, a ROM file (`.gb`) is simply a binary file containing thousands of bytes.

```
Address    Data

0000       31
0001       FE
0002       FF
0003       AF
...
0100       00
0101       C3
0102       50
...
```

Each byte represents either:

- An instruction (opcode)
- Data used by the game
- Graphics
- Sound information
- Game resources

The CPU simply reads these bytes one by one during execution.

---

## Cartridge Header

Every Game Boy cartridge contains a header located near the beginning of the ROM.

```
Address Range

0x0100 - 0x014F
```

This header contains important information about the game.

Some important fields include:

| Address | Description |
|----------|-------------|
| 0x0134 - 0x0143 | Game Title |
| 0x0147 | Cartridge Type |
| 0x0148 | ROM Size |
| 0x0149 | RAM Size |

Instead of hardcoding this information, the emulator reads these bytes and interprets them.

For example,

```
Title : TETRIS
Type  : ROM ONLY
ROM   : 32 KB
```

Displaying this information is useful for debugging and verifying that the ROM has been loaded correctly.

---

## Why Doesn't the CPU Start at Address 0x0000?

One question that came up during development was:

> If the ROM starts at address `0x0000`, why is the CPU initialized to `0x0100`?

The answer is the **Boot ROM**.

When a real Game Boy powers on, it first executes a small built-in program called the Boot ROM.

The Boot ROM:

- Displays the Nintendo logo
- Verifies the cartridge
- Initializes hardware registers
- Transfers control to the game

After the Boot ROM finishes, execution jumps to:

```
0x0100
```

Since the emulator skips the Boot ROM, the Program Counter is initialized directly to `0x0100`.

---

## Loading the ROM

The first responsibility of the Cartridge class is loading the ROM file into memory.

The ROM is opened as a binary file and every byte is copied into a vector.

Conceptually:

```cpp
Open ROM file

↓

Read bytes

↓

Store bytes inside romData
```

Using a `std::vector<uint8_t>` allows ROMs of different sizes to be loaded without manually managing memory.

---

## Reading Data from the Cartridge

Once the ROM has been loaded, the CPU must be able to read bytes from it.

Whenever the CPU accesses an address within the ROM region,

```
0x0000
↓

0x7FFF
```

the Memory class forwards the request to the Cartridge.

```
CPU

↓

Memory::readByte()

↓

Cartridge::readByte()

↓

ROM Data
```

This separation keeps the CPU independent from the cartridge implementation.

The CPU only knows that it is reading memory—it does not know where that memory actually comes from.

---

## Our Cartridge Class

The Cartridge class was designed with three primary responsibilities.

### 1. Load the ROM

Reads the ROM file from disk.

### 2. Parse the Header

Extracts useful information such as:

- Game title
- Cartridge type
- ROM size

### 3. Provide ROM Data

Returns bytes requested by the Memory class during execution.

Keeping these responsibilities together makes the Cartridge class simple and self-contained.

---

## Design Decisions

Several design decisions were made while implementing the Cartridge class.

### Store the ROM inside a vector

Instead of allocating memory manually, the ROM is stored using:

```cpp
std::vector<uint8_t> romData;
```

Advantages:

- Automatic memory management
- Supports ROMs of different sizes
- Easy indexing using addresses

---

### Keep Header Parsing Separate

Rather than reading header information throughout the emulator, it is parsed once when the ROM is loaded.

This prevents duplicate code and keeps cartridge-specific logic inside the Cartridge class.

---

## Current Limitations

At the current stage of development, the emulator primarily supports ROMs without advanced memory bank controllers.

Support for additional cartridge hardware (MBC1, MBC3, MBC5, etc.) can be added later to improve compatibility with larger commercial games.

---

## Lessons Learned

- A Game Boy ROM is simply a binary file containing machine code and game data.
- The cartridge header provides essential information required by the emulator.
- The CPU begins execution at `0x0100` because the Boot ROM has already completed its initialization.
- The Cartridge class should only be responsible for loading ROM data and exposing it to the rest of the emulator.
- Separating cartridge handling from memory management keeps the emulator modular and easier to maintain.