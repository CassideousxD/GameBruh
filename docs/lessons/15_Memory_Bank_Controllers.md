# Memory Bank Controllers (MBC)

## Introduction

As Game Boy games became larger and more sophisticated, the limitations of the original cartridge design became increasingly apparent. The Game Boy CPU could only directly access 32 KB of ROM, while newer games required hundreds of kilobytes or even several megabytes of storage.

Rather than redesigning the console, Nintendo introduced **Memory Bank Controllers (MBCs)**. These controllers acted as intelligent hardware inside the cartridge, allowing different portions of ROM or RAM to be mapped into the CPU's address space when required.

Over time, several MBC variants were released, each introducing additional capabilities such as larger ROM support, external RAM, battery-backed saves, and real-time clocks.

---

# ROM Only

The simplest cartridge contains no Memory Bank Controller.

```
CPU
        │
        ▼
+----------------+
| 32 KB ROM      |
+----------------+
```

The entire ROM is permanently mapped into memory.

```
0000 - 3FFF  → ROM
4000 - 7FFF  → ROM
```

Since nothing can be switched, the maximum ROM size is limited to:

```
32 KB
```

### Advantages

- Simple hardware
- Easy to emulate
- Low manufacturing cost

### Limitations

- Maximum ROM size of 32 KB
- No external RAM
- No save functionality
- No additional hardware features

ROM Only cartridges were commonly used for early Game Boy titles and test ROMs.

---

# MBC1

As games became larger, 32 KB was no longer sufficient.

MBC1 introduced **ROM bank switching**, allowing cartridges to contain much larger ROMs.

```
0000 - 3FFF

↓

Fixed Bank 0
```

```
4000 - 7FFF

↓

Switchable Bank
```

Instead of permanently mapping one ROM bank, the cartridge can replace the second region with different banks.

For example,

```
Bank 1

↓

Bank 2

↓

Bank 17

↓

Bank 42
```

while the CPU continues reading addresses

```
4000 - 7FFF
```

without knowing the difference.

### Additional Features

- ROM bank switching
- External RAM support
- RAM bank switching
- Battery-backed saves (optional)

### Banking Modes

MBC1 supports two operating modes.

### ROM Banking Mode

Most memory is allocated for ROM.

Suitable for games requiring larger ROM sizes.

---

### RAM Banking Mode

Some ROM banking capacity is sacrificed in exchange for additional RAM banks.

Useful for games requiring larger save memory.

---

### Limitations

Although significantly more capable than ROM Only cartridges, MBC1 still has restrictions on the maximum ROM and RAM sizes it can support.

---

# MBC2

MBC2 was designed for games requiring save functionality without needing a separate external RAM chip.

Unlike MBC1,

MBC2 contains **built-in RAM** inside the controller itself.

```
512 × 4-bit RAM
```

Instead of storing a full byte,

each memory location stores only

```
4 bits
```

This small amount of RAM is sufficient for many save files while reducing manufacturing cost.

### Features

- ROM bank switching
- Built-in RAM
- Battery support (optional)

### Limitations

- Very small save memory
- No external RAM expansion

MBC2 was commonly used in games with relatively simple save requirements.

---

# MBC3

MBC3 expanded cartridge capabilities beyond simple memory banking.

In addition to larger ROM and RAM support, it introduced a **Real-Time Clock (RTC).**

The RTC continuously keeps track of real-world time, even when the Game Boy is turned off.

It records values such as:

- Seconds
- Minutes
- Hours
- Days

This is made possible by powering the RTC using the cartridge battery.

### Applications

Games can implement features such as:

- Day and night cycles
- Daily events
- Timed challenges
- Seasonal changes

A well-known example is the Pokémon series, where different Pokémon appear depending on the current time of day.

### Features

- Large ROM support
- External RAM
- Battery-backed saves
- Real-Time Clock

MBC3 became one of the most widely used cartridge controllers for advanced Game Boy games.

---

# MBC5

MBC5 represents one of the most advanced Memory Bank Controllers developed for the Game Boy.

It significantly increased the maximum supported ROM size and improved bank switching capabilities.

Unlike previous controllers, MBC5 uses a **9-bit ROM bank number**, allowing access to hundreds of ROM banks.

This makes it suitable for very large Game Boy and Game Boy Color games.

### Features

- Very large ROM support
- Large external RAM support
- Battery-backed saves
- Optional rumble support

Because of its expanded capabilities, MBC5 became the preferred controller for many later Game Boy titles.

---

# Comparing the Controllers

| Feature | ROM Only | MBC1 | MBC2 | MBC3 | MBC5 |
|----------|:--------:|:----:|:----:|:----:|:----:|
| ROM Bank Switching | ❌ | ✅ | ✅ | ✅ | ✅ |
| External RAM | ❌ | ✅ | ❌ | ✅ | ✅ |
| Built-in RAM | ❌ | ❌ | ✅ | ❌ | ❌ |
| Battery Saves | ❌ | Optional | Optional | Optional | Optional |
| Real-Time Clock | ❌ | ❌ | ❌ | ✅ | ❌ |
| Rumble Support | ❌ | ❌ | ❌ | ❌ | Optional |

---

# Selecting the Correct Controller

Every Game Boy ROM contains a **Cartridge Type** field in its header.

When the emulator loads a ROM, it reads this value to determine which Memory Bank Controller should be used.

Conceptually, the process is:

```
Load ROM

↓

Read Cartridge Header

↓

Determine Cartridge Type

↓

Create Appropriate MBC

↓

Execute Game
```

This allows the emulator to support many different cartridge types while presenting a consistent interface to the CPU.

---

# Key Takeaways

- Memory Bank Controllers were introduced to overcome the 32 KB ROM limitation of the original Game Boy.
- Bank switching allows the CPU to access different portions of a larger ROM without changing its address space.
- MBC1 introduced ROM and RAM bank switching.
- MBC2 includes a small amount of built-in RAM for save data.
- MBC3 adds a Real-Time Clock for time-based gameplay.
- MBC5 supports very large ROMs and optional hardware features such as rumble.
- Modern Game Boy emulators must identify the cartridge type before execution so that the correct controller behavior can be emulated.