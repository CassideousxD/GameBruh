# PPU Basics

## Introduction

The **Picture Processing Unit (PPU)** is the hardware responsible for generating everything displayed on the Game Boy screen.

Unlike modern graphics processors, the Game Boy does not draw an entire frame at once. Instead, it builds the image one horizontal line at a time, continuously updating the display as the screen is refreshed.

The PPU works alongside the CPU throughout the execution of a game. While the CPU executes instructions, the PPU independently generates pixels based on data stored in memory.

---

# What is the PPU?

The PPU (Picture Processing Unit) is a specialized graphics processor.

Its responsibilities include:

- Rendering the background
- Rendering the window
- Rendering sprites
- Managing LCD timing
- Tracking the current scanline
- Generating graphics-related interrupts

The CPU never draws pixels directly.

Instead, the CPU writes data into memory, and the PPU reads that data to generate the final image displayed on the screen.

This separation allows both processors to work simultaneously.

---

# Screen Resolution

The original Nintendo Game Boy has a screen resolution of:

```
160 × 144 pixels
```

This means every frame consists of:

- 160 pixels horizontally
- 144 visible scanlines vertically

Although only 144 lines are visible, the Game Boy actually processes **154 scanlines** every frame.

```
Visible Lines

0
│
│
143

↓

VBlank

144
145
146
...
153
```

The last ten scanlines occur during **Vertical Blank (VBlank)** before rendering begins again from line 0.

---

# Scanlines

Instead of drawing every pixel simultaneously, the Game Boy renders one horizontal row of pixels at a time.

Each horizontal row is called a **scanline**.

For every frame, the PPU performs the following sequence:

```
Draw Scanline 0

↓

Draw Scanline 1

↓

Draw Scanline 2

↓

...

↓

Draw Scanline 143

↓

Enter VBlank

↓

Start Again
```

Rendering line by line greatly reduced hardware complexity compared to drawing the entire screen at once.

---

# LCD Modes

Each scanline is divided into several stages known as **LCD Modes**.

The Game Boy cycles through these modes continuously while rendering.

```
Mode 2

↓

Mode 3

↓

Mode 0

↓

Next Scanline

↓

...

↓

Mode 1 (VBlank)
```

Each mode has a specific responsibility.

---

## Mode 2 – OAM Search

Duration:

```
80 cycles
```

During this stage, the PPU searches Object Attribute Memory (OAM) to determine which sprites will appear on the current scanline.

No pixels are drawn yet.

The hardware is simply preparing the data required for rendering.

---

## Mode 3 – Pixel Transfer

Duration:

```
172 cycles
```

This is the actual drawing stage.

The PPU reads:

- Tile data
- Tile maps
- Sprite information

and determines the color of every pixel on the current scanline.

By the end of this stage, one complete scanline has been generated.

---

## Mode 0 – HBlank

Duration:

```
204 cycles
```

After completing a scanline, the PPU enters **Horizontal Blank (HBlank)**.

No pixels are drawn during this period.

Instead, the hardware prepares to begin rendering the next scanline.

Many games use HBlank to safely update graphics data.

---

## Mode 1 – VBlank

After the final visible scanline has been rendered, the PPU enters **Vertical Blank (VBlank)**.

```
Scanline 144

↓

145

↓

146

↓

...

↓

153
```

During VBlank:

- No visible pixels are drawn.
- The display shows the completed frame.
- A VBlank interrupt is generated.

Games commonly use this period to update graphics without causing visible artifacts on the screen.

---

# Frame Timing

Each scanline always requires:

```
456 CPU cycles
```

This consists of:

```
80

+

172

+

204

=

456 cycles
```

Since there are:

```
154 scanlines
```

one complete frame requires:

```
456 × 154

=

70,224 CPU cycles
```

This fixed timing allows every Game Boy to refresh the screen at a consistent frame rate.

---

# Synchronization with the CPU

The CPU and PPU operate independently but remain synchronized using CPU cycles.

Every instruction executed by the CPU consumes a certain number of cycles.

Those cycles are also used to advance the PPU.

For example,

```
CPU executes instruction

↓

Consumes 8 cycles

↓

PPU advances by 8 cycles

↓

Timer advances by 8 cycles

↓

Audio advances by 8 cycles
```

Because every hardware component advances together, the emulator accurately reproduces the timing of the original Game Boy.

---

# Memory Used by the PPU

The PPU reads graphical information from dedicated memory regions.

| Address Range | Purpose |
|---------------|---------|
| 8000–97FF | Tile Data |
| 9800–9BFF | Background Tile Map |
| 9C00–9FFF | Window Tile Map |
| FE00–FE9F | Sprite Attribute Table (OAM) |

The CPU writes graphics information into these memory regions, while the PPU continuously reads from them during rendering.

The details of these memory regions will be discussed in the following chapters.

---

# Key Takeaways

- The PPU is responsible for generating every frame displayed on the screen.
- The Game Boy renders graphics one scanline at a time rather than drawing the entire frame at once.
- Every scanline progresses through four LCD modes: OAM Search, Pixel Transfer, HBlank, and VBlank.
- A complete scanline always requires 456 CPU cycles.
- A complete frame consists of 154 scanlines.
- The CPU, Timer, PPU, and Audio system remain synchronized using CPU cycles.
- The PPU reads graphics information from dedicated regions of memory instead of generating graphics directly.