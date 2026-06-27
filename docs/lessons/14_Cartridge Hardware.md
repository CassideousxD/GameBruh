# Cartridge Hardware

## Introduction

Early Game Boy games were relatively small and could fit entirely within the CPU's addressable ROM space. As games became more complex, developers required larger ROMs, additional save memory, and eventually new hardware features such as real-time clocks and rumble motors.

To overcome the limitations of the Game Boy's memory architecture, Nintendo introduced additional hardware inside game cartridges. These components extended the capabilities of the console without requiring any modifications to the Game Boy itself.

Understanding cartridge hardware is essential because the cartridge is more than just a storage device—it actively participates in the execution of the game.

---

# ROM Address Space

The Game Boy CPU has a 16-bit address bus, allowing it to access:

```
2^16 = 65,536 bytes
```

or

```
64 KB
```

of memory at any given moment.

However, not all of this memory is available for ROM.

The cartridge ROM occupies only:

```
0000 - 7FFF
```

which corresponds to:

```
32 KB
```

This means the CPU can directly access only 32 KB of ROM at one time.

---

# The Problem

As games evolved, they quickly exceeded the 32 KB ROM limit.

For example,

| Game | Approximate ROM Size |
|-------|---------------------:|
| Tetris | 32 KB |
| Pokémon Red | 1 MB |
| Pokémon Crystal | 2 MB |
| The Legend of Zelda: Link's Awakening DX | 1 MB |

Clearly, these games could not fit inside the CPU's directly accessible ROM region.

Nintendo needed a solution without redesigning the Game Boy hardware.

---

# Bank Switching

The solution was **bank switching**.

Instead of increasing the CPU's address space, cartridges dynamically changed which portion of the ROM appeared within part of the address space.

The ROM region is divided into two sections.

```
0000 - 3FFF

↓

Fixed ROM Bank
```

```
4000 - 7FFF

↓

Switchable ROM Bank
```

The first region always contains the beginning of the game.

The second region can be changed to display different portions of the ROM.

Conceptually,

```
Initially

4000-7FFF

↓

Bank 1
```

Later,

```
4000-7FFF

↓

Bank 5
```

Later,

```
4000-7FFF

↓

Bank 18
```

Although the CPU always reads the same addresses, the cartridge changes the physical ROM connected to those addresses.

This technique allows games much larger than 32 KB to execute correctly.

---

# Memory Bank Controllers (MBCs)

The hardware responsible for bank switching is called a **Memory Bank Controller (MBC).**

An MBC is an integrated circuit located inside the cartridge.

Its responsibilities include:

- Switching ROM banks
- Switching RAM banks
- Enabling or disabling external RAM
- Managing battery-backed saves
- Supporting additional cartridge features

Different cartridges contain different MBCs depending on the game's requirements.

---

# External RAM

Some games need to store data while the game is running.

Examples include:

- Save files
- Player progress
- Inventory
- High scores

Instead of storing this information inside ROM, cartridges may include **External RAM**.

Unlike ROM,

External RAM can be written to during gameplay.

This memory typically occupies:

```
A000 - BFFF
```

within the Game Boy's address space.

---

# Battery-Backed Saves

RAM normally loses its contents when power is removed.

To preserve save data, many Game Boy cartridges include a small battery.

The battery continuously powers the external RAM even after the console is turned off.

As a result,

- saved games,
- player progress,
- and configuration data

remain available the next time the cartridge is used.

Without the battery, save data would disappear every time the Game Boy lost power.

---

# Real-Time Clock (RTC)

Some cartridges include a **Real-Time Clock (RTC).**

Unlike ordinary RAM, an RTC continuously tracks:

- seconds
- minutes
- hours
- days

even while the Game Boy is switched off.

Games use this feature for events that depend on real-world time.

Examples include:

- day and night cycles
- timed events
- daily rewards
- seasonal changes

The RTC operates independently of the CPU using its own clock source.

---

# Additional Cartridge Features

As cartridge technology evolved, additional hardware was introduced.

Some cartridges may include:

- Larger ROM capacity
- Larger RAM capacity
- Real-Time Clock
- Rumble motors
- Accelerometers
- Sensors

Although the Game Boy itself never changed, these cartridge enhancements significantly expanded the capabilities of later games.

---

# Cartridge Types

Every Game Boy ROM contains a **Cartridge Type** value inside its header.

This value tells the emulator which hardware exists inside the cartridge.

Examples include:

- ROM ONLY
- MBC1
- MBC2
- MBC3
- MBC5

When the emulator loads a ROM, it reads this value and selects the appropriate cartridge hardware implementation.

Without identifying the cartridge type correctly, many commercial games would fail to run.

---

# Key Takeaways

- The CPU can directly access only 32 KB of cartridge ROM at a time.
- As games became larger, bank switching was introduced to overcome this limitation.
- Memory Bank Controllers (MBCs) manage ROM and RAM bank switching.
- External RAM allows games to store save data and other writable information.
- Battery-backed cartridges preserve save data when the console is powered off.
- Some cartridges include additional hardware such as Real-Time Clocks and rumble motors.
- Correctly identifying the cartridge type is essential for accurate emulation.