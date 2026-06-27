# Audio Processing Unit (APU)

## Introduction

The **Audio Processing Unit (APU)** is the hardware responsible for generating all sound produced by the Game Boy. While the CPU executes game logic and the PPU renders graphics, the APU continuously produces audio based on values written to its control registers.

The CPU does not generate sound directly. Instead, it configures the APU by writing to specific memory-mapped registers. The APU then produces audio independently until its settings are changed.

This separation allows the CPU to continue executing the game while sound is generated simultaneously.

---

# Why Does the Game Boy Need an APU?

Imagine a game where the CPU had to calculate every individual audio sample while also handling graphics, input, timers, and game logic.

This would significantly increase the CPU's workload.

Instead, the Game Boy includes dedicated audio hardware capable of generating sound automatically.

The CPU simply tells the APU:

- which sound to play,
- its frequency,
- its volume,
- and how long it should play.

The APU handles the actual waveform generation.

---

# The Four Sound Channels

The original Game Boy contains four independent sound channels.

Each channel is designed for a different purpose.

| Channel | Description |
|---------|-------------|
| Channel 1 | Square Wave with Frequency Sweep |
| Channel 2 | Square Wave |
| Channel 3 | Programmable Wave Channel |
| Channel 4 | Noise Generator |

All four channels operate simultaneously and are mixed together to produce the final audio output.

---

## Channel 1 – Square Wave with Sweep

Channel 1 generates a square wave.

In addition to producing sound, it can automatically change its frequency over time using a feature called **frequency sweep**.

This effect is commonly used for:

- Laser sounds
- Power-up effects
- Rising and falling tones

---

## Channel 2 – Square Wave

Channel 2 also generates a square wave but does not support frequency sweep.

It is typically used for:

- Background melodies
- Musical notes
- Simple sound effects

---

## Channel 3 – Wave Channel

Unlike the first two channels, Channel 3 does not generate a predefined waveform.

Instead, it reads waveform data from a small area of memory called **Wave RAM**.

This allows games to play custom sounds that are more complex than simple square waves.

---

## Channel 4 – Noise Channel

The fourth channel generates pseudo-random noise instead of musical tones.

It is commonly used for:

- Explosions
- Drum sounds
- Static effects
- Impacts

Although it sounds random, the noise is generated using predictable hardware logic.

---

# Memory-Mapped Audio Registers

Like most Game Boy hardware, the APU is controlled through memory-mapped registers.

These registers allow the CPU to configure:

- Frequency
- Volume
- Channel enable/disable
- Sound length
- Stereo output

Instead of calling audio functions directly, games simply write values into these registers.

The APU continuously reads these values while generating sound.

---

# Sound Generation

Each channel continuously produces a waveform based on its configuration.

The APU combines the outputs of all active channels into a single audio signal.

Conceptually, the process looks like:

```
Channel 1

      ↓

Channel 2

      ↓

Channel 3

      ↓

Channel 4

      ↓

Audio Mixer

      ↓

Speaker
```

The player hears only the final mixed output.

---

# Synchronization

Like every other hardware component, the APU must remain synchronized with the CPU.

As the CPU executes instructions:

- the Timer advances,
- the PPU updates graphics,
- and the APU advances its internal state.

If audio is generated too quickly or too slowly, noticeable problems occur such as:

- distorted sound,
- incorrect pitch,
- audio glitches,
- or synchronization issues with gameplay.

Accurate timing is therefore just as important for audio as it is for graphics.

---

# Why the CPU Doesn't Play Sound Directly

A common misconception is that the CPU produces sound.

In reality, the CPU only configures the APU.

For example,

```
CPU

↓

Write Frequency Register

↓

Write Volume Register

↓

Enable Channel

↓

Continue Executing Instructions
```

The APU then continues generating sound independently until another configuration change occurs.

This allows the CPU to focus on game logic instead of continuously calculating audio.

---

# Relationship with Other Hardware

The APU works alongside the other major hardware components.

```
CPU

↓

Memory

↓

APU Registers

↓

Wave Generation

↓

Audio Output
```

The CPU communicates with the APU through memory, just as it communicates with the Timer, PPU, and Joypad.

This consistent design is one of the reasons the Game Boy hardware is relatively elegant and easy to understand.

---

# Key Takeaways

- The APU is responsible for generating all Game Boy audio.
- The CPU controls the APU by writing to memory-mapped registers.
- The Game Boy contains four independent sound channels, each designed for different types of audio.
- All channels are mixed together to produce the final output.
- Audio generation occurs independently of CPU execution.
- Correct synchronization between the CPU and APU is essential for accurate sound reproduction.