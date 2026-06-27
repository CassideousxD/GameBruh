# Backgrounds and Window

## Introduction

Unlike modern computers, the Game Boy does not store the screen as a large collection of individual pixels. Instead, it constructs every frame by combining many small images called **tiles**.

These tiles are reused repeatedly to create backgrounds, menus, text, and other graphical elements. This approach significantly reduces memory usage while allowing large game worlds to be displayed using a relatively small amount of graphical data.

To determine which tiles should appear on the screen, the Game Boy uses structures known as **tile maps**. During rendering, the PPU reads tile information from memory, assembles the tiles into a complete background, and finally displays the visible portion on the screen.

---

# Why Use Tiles?

Consider a screen with a resolution of:

```
160 × 144 pixels
```

If every pixel were stored individually, a large amount of memory would be required.

However, many graphical elements repeat frequently.

Examples include:

- Grass
- Bricks
- Water
- Trees
- Letters
- Numbers

Instead of storing these graphics multiple times, the Game Boy stores them once as tiles and simply reuses them wherever needed.

This greatly reduces memory consumption and makes rendering much more efficient.

---

# What is a Tile?

A tile is a small image measuring:

```
8 × 8 pixels
```

Each tile contains:

```
8 rows

↓

8 pixels per row
```

Visually,

```
□□□□□□□□
□□□□□□□□
□□□□□□□□
□□□□□□□□
□□□□□□□□
□□□□□□□□
□□□□□□□□
□□□□□□□□
```

Every background, menu, and window displayed by the Game Boy is built by arranging these small blocks together.

---

# Tile Data

The actual graphical appearance of each tile is stored inside **Tile Data** memory.

Tile Data answers the question:

> **"What does this tile look like?"**

Each tile occupies exactly:

```
16 bytes
```

This is because:

- Each tile has 8 rows.
- Each row requires 2 bytes.
- Therefore,

```
8 × 2 = 16 bytes
```

The PPU reads these bytes and converts them into visible pixels.

---

# How Are Pixels Stored?

Each row of a tile is represented using **two bytes**.

Example:

```
Byte 1

10110010

Byte 2

01101001
```

The corresponding bits from both bytes are combined to determine the color of each pixel.

Each pixel is therefore represented using **2 bits**.

Possible values are:

```
00

↓

Color 0

01

↓

Color 1

10

↓

Color 2

11

↓

Color 3
```

Since only two bits are used per pixel, the original Game Boy can display four shades of gray.

---

# Tile Maps

Tile Data stores the graphics.

However, the Game Boy still needs to know **where each tile should appear**.

This information is stored in the **Tile Map**.

Think of the Tile Map as a blueprint.

Instead of storing images,

it stores **tile numbers**.

Example:

```
03 03 03 03
05 06 07 08
05 06 07 08
01 02 01 02
```

Each value refers to a tile stored inside Tile Data.

The PPU reads the Tile Map first, then retrieves the corresponding tile graphics before drawing them.

---

# Background Layer

The background is simply a large arrangement of tiles.

Internally, the Game Boy maintains a background measuring:

```
256 × 256 pixels
```

This is much larger than the visible screen.

Only a small portion:

```
160 × 144
```

is displayed at any given time.

The remainder exists outside the visible area and becomes visible as the player moves through the game world.

---

# Scrolling

Since the background is larger than the display, the Game Boy needs a way to choose which portion should be visible.

This is achieved using two registers:

```
SCX

↓

Horizontal Scroll
```

```
SCY

↓

Vertical Scroll
```

By changing these values, the visible section of the background moves.

Importantly, the background itself does not move.

Instead, the **viewing position** changes.

This creates the illusion that the player is moving through a much larger world.

---

# The Window Layer

Despite its name, the **Window** is not a pop-up window like those found in modern operating systems.

It is simply another tile layer drawn on top of the background.

Games commonly use the Window for:

- Menus
- Dialogue boxes
- Status bars
- Inventory screens

Unlike the background, the Window does not scroll with the game world.

Instead, it remains fixed relative to the screen.

---

# Window Position

The Window position is controlled using two registers.

```
WX

↓

Window X Position
```

```
WY

↓

Window Y Position
```

These registers determine where the Window begins to appear on the display.

Everything above or to the left of this position continues to display the background.

Everything inside the Window region is drawn using the Window Tile Map.

---

# Tile Addressing Modes

The Game Boy supports two methods of locating tile data.

### Unsigned Addressing

Tile numbers range from:

```
0

↓

255
```

Each value directly identifies a tile.

---

### Signed Addressing

Some LCD configurations interpret tile numbers as signed values.

Instead of:

```
0 → 255
```

the range becomes:

```
-128 → 127
```

Although the tile graphics remain the same, the starting address used to locate them changes.

The selected addressing mode is determined by the LCD Control Register.

---

# Rendering the Background

To render a single pixel, the PPU performs several steps.

1. Determine the current screen position.
2. Apply horizontal and vertical scrolling.
3. Locate the corresponding tile inside the Tile Map.
4. Retrieve that tile from Tile Data.
5. Decode the required row.
6. Determine the pixel's color.
7. Draw the pixel.

This process is repeated for every visible pixel on every scanline.

Although the algorithm is straightforward, it executes thousands of times every frame.

---

# Key Takeaways

- The Game Boy constructs graphics using reusable 8×8 pixel tiles.
- Tile Data stores the appearance of each tile.
- Tile Maps determine where tiles appear on the screen.
- The background is larger than the visible display, allowing games to scroll.
- SCX and SCY control which portion of the background is currently visible.
- The Window is an independent tile layer used for user interface elements.
- WX and WY determine the Window's position on the screen.
- Every visible pixel is generated by combining information from the Tile Map, Tile Data, and scrolling registers.