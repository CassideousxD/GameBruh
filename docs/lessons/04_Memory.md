# Memory

## Overview

The Memory class is one of the most important components of the emulator. Every hardware component communicates through memory, making it the central hub of the entire system.

During development, I initially assumed memory would simply be a 64 KB array that stores bytes. However, I quickly learned that the Game Boy's memory is far more complex. Many memory addresses do not store ordinary data; instead, they represent hardware components such as the LCD controller, timers, joypad, audio registers, and interrupt flags.

Because of this, the Memory class became responsible not only for storing data but also for routing read and write operations to the correct hardware component.

---

## The Game Boy Memory Map

The Game Boy has a 16-bit address bus, allowing it to access 65,536 memory locations.

```
0000 - 3FFF   ROM Bank 0
4000 - 7FFF   Switchable ROM Bank
8000 - 9FFF   Video RAM (VRAM)
A000 - BFFF   External RAM
C000 - DFFF   Work RAM (WRAM)
E000 - FDFF   Echo RAM
FE00 - FE9F   Object Attribute Memory (Sprites)
FEA0 - FEFF   Unusable
FF00 - FF7F   I/O Registers
FF80 - FFFE   High RAM (HRAM)
FFFF          Interrupt Enable Register
```

Every address has a specific purpose. Accessing different address ranges produces different behavior.

For example,

Writing to `0xC000` stores data in Work RAM, while writing to `0xFF44` changes the LCD scanline register.

---

## Designing the Memory Class

The first version of the Memory class simply contained a 64 KB array.

```cpp
uint8_t memory[65536];
```

This array represents every address from `0x0000` to `0xFFFF`.

However, reading directly from this array would not accurately emulate the Game Boy because many addresses require special handling.

To solve this, all memory access is performed through two functions:

```cpp
uint8_t Memory::readByte(uint16_t address);

void Memory::writeByte(uint16_t address, uint8_t value);
```

Every hardware component uses these functions instead of directly accessing the memory array.

---

## Reading Memory

Whenever the CPU executes an instruction that needs data, it calls:

```cpp
memory.readByte(address);
```

The Memory class first determines which hardware component owns that address.

For example,

```
Address < 0x8000

↓

Cartridge ROM

↓

Return byte from Cartridge
```

If the address belongs to ordinary RAM, the value is returned directly from the memory array.

This routing mechanism allows different hardware components to behave correctly without the CPU needing to know where the data actually comes from.

---

## Writing Memory

Writing follows the same principle.

Instead of directly modifying the array,

```cpp
memory[address] = value;
```

every write passes through:

```cpp
memory.writeByte(address, value);
```

The function determines whether the write affects:

- Work RAM
- Video RAM
- Hardware registers
- DMA
- Joypad
- Timer
- LCD Controller

Each address range performs its own operation.

This makes `writeByte()` one of the busiest functions in the emulator.

---

## Connecting the Cartridge

Since ROM data is stored inside the Cartridge class, the Memory class needs access to it.

To achieve this, a connection function was added.

```cpp
connectCartridge(Cartridge* cartridge);
```

This allows the Memory class to forward ROM reads to the cartridge whenever the CPU accesses addresses within the ROM region.

Without this connection, the CPU would have no way of reading instructions from the game.

---

## Hardware Register Initialization

A real Game Boy powers on with many hardware registers already containing predefined values.

Initially, these registers were left uninitialized, causing several games to behave incorrectly.

To solve this, the function

```cpp
initializeHardwareRegisters();
```

was introduced.

This function assigns the default values expected after the Boot ROM finishes execution.

Examples include timer registers, LCD control registers, sound registers, and interrupt flags.

Initializing these registers significantly improved ROM compatibility.

---

## DMA Transfer

Sprites are stored inside Object Attribute Memory (OAM).

Instead of copying sprite data byte by byte, the Game Boy provides Direct Memory Access (DMA).

Whenever the CPU writes to:

```
0xFF46
```

the Memory class performs a DMA transfer by calling:

```cpp
doDMATransfer(uint8_t value);
```

The source address is calculated from the value written, and 160 bytes are copied into OAM.

Implementing DMA allowed sprite data to be transferred exactly as expected by commercial games.

---

## Special Memory Addresses

Not every address behaves like normal memory.

Several addresses required dedicated handling inside `readByte()` and `writeByte()`.

Examples include:

### FF00

Joypad input register.

Reading this address depends on which button group is currently selected.

---

### FF04

Divider register (DIV).

Writing any value resets the divider instead of storing the value.

---

### FF40

LCD Control register.

Controls whether the LCD is enabled along with several rendering options.

---

### FF44

Current scanline (LY).

Updated continuously by the PPU during rendering.

---

### FF46

DMA register.

Writing to this address immediately starts a DMA transfer.

These special cases reinforced that memory addresses can represent hardware behavior rather than simple storage.

---

## Problems Encountered

One of the biggest challenges during development was assuming every memory address behaved identically.

As more hardware components were implemented, additional conditions had to be added to `readByte()` and `writeByte()` to correctly emulate hardware behavior.

Another challenge involved debugging incorrect ROM execution.

Several CPU bugs were eventually traced back to memory rather than instruction implementation.

This highlighted how closely every subsystem depends on correct memory behavior.

---

## Lessons Learned

- Memory is the communication hub of the emulator.
- Every hardware component interacts through memory addresses.
- Not every memory location stores ordinary data; many represent hardware registers.
- Centralizing all memory access through `readByte()` and `writeByte()` greatly simplified debugging.
- Proper hardware register initialization is essential for running commercial Game Boy ROMs.
- DMA transfers and memory-mapped I/O are critical features that significantly improve hardware accuracy.