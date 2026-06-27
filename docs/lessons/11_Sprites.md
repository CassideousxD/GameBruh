# Sprites

## Introduction

Sprites are movable graphical objects that are rendered independently of the background and window. Unlike background tiles, which form the game world, sprites are intended for objects that move or change frequently, such as the player, enemies, projectiles, items, and cursors.

The Game Boy treats sprites differently from background tiles because they require additional information, such as position, orientation, and rendering priority.

---

# What is a Sprite?

A sprite is simply a tile (or two tiles) that can be placed anywhere on the screen.

Unlike the background, sprites are not fixed to the tile map.

They have their own:

- Position
- Tile number
- Rendering priority
- Display attributes

For example, in a platform game:

- The ground is part of the background.
- The player is a sprite.
- Enemies are sprites.
- Coins are sprites.

---

# Object Attribute Memory (OAM)

All sprite information is stored inside a dedicated memory region called **Object Attribute Memory (OAM)**.

Memory Range:

```
FE00 - FE9F
```

The Game Boy supports a maximum of:

```
40 sprites
```

Each sprite occupies:

```
4 bytes
```

Therefore,

```
40 × 4 = 160 bytes
```

which exactly matches the size of the OAM memory region.

---

# Sprite Attributes

Each sprite consists of four bytes.

| Byte | Purpose |
|------|---------|
| Byte 1 | Y Position |
| Byte 2 | X Position |
| Byte 3 | Tile Number |
| Byte 4 | Attribute Flags |

Together, these four values completely describe a sprite.

---

## Position

The first two bytes determine where the sprite appears on the screen.

```
Y Position

↓

Vertical location
```

```
X Position

↓

Horizontal location
```

Changing these values moves the sprite without modifying its graphics.

This makes animation much more efficient than redrawing graphics every frame.

---

## Tile Number

The third byte specifies which tile should be used to draw the sprite.

The graphics themselves are stored in Tile Data.

The sprite simply references the required tile.

This allows multiple sprites to reuse the same graphics while appearing in different locations.

---

## Attribute Flags

The final byte stores several properties that affect how the sprite is rendered.

These include:

- Rendering priority
- Horizontal flip
- Vertical flip
- Palette selection

Instead of storing each property separately, they are packed into individual bits of a single byte.

This is another example of efficient memory usage.

---

# Sprite Sizes

The Game Boy supports two sprite sizes.

### 8 × 8

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

One tile is used.

---

### 8 × 16

```
□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□

□□□□□□□□
```

Two vertically adjacent tiles are combined to form a larger sprite.

The selected size is controlled through the LCD Control register.

---

# Transparency

Sprites do not completely replace the background.

Instead, one of the four available colors is treated as **transparent**.

Transparent pixels are skipped during rendering.

This allows irregular sprite shapes without requiring additional memory.

For example,

A character standing in front of the background only draws its visible pixels.

Everything else allows the background to remain visible.

---

# Sprite Priority

Sometimes sprites overlap other graphics.

The Game Boy must decide which pixels should appear on top.

Priority rules determine whether:

- The sprite appears in front of the background.
- The background appears in front of the sprite.

Certain background colors can also hide portions of a sprite depending on its priority settings.

This mechanism creates the illusion that characters move behind walls or objects.

---

# Flipping Sprites

Instead of storing separate graphics for every direction, sprites can be mirrored.

Two types of flipping are supported.

### Horizontal Flip

```
Normal

ABCDEF

↓

FEDCBA
```

---

### Vertical Flip

```
Normal

A

B

C

↓

C

B

A
```

Flipping allows games to reuse the same graphics for left-facing and right-facing characters, saving valuable ROM space.

---

# Sprite Palettes

The original Game Boy supports four grayscale colors.

Sprites can choose between different palettes to determine how these colors should appear.

Although the graphics remain identical, different palette selections allow sprites to appear visually distinct.

---

# Sprite Rendering Limits

Although the Game Boy supports forty sprites in memory, it cannot display all of them simultaneously on every scanline.

The hardware can render:

```
Maximum 10 sprites per scanline
```

If more than ten sprites overlap the same horizontal line, only the highest-priority sprites are displayed.

This limitation was imposed by the hardware and can occasionally be observed in games as sprite flickering.

Many developers intentionally alternated sprite priorities each frame to make the flickering less noticeable.

---

# Rendering Process

For every visible scanline, the PPU performs the following steps:

1. Search OAM for sprites that intersect the current scanline.
2. Select up to ten eligible sprites.
3. Retrieve each sprite's tile data.
4. Apply horizontal or vertical flipping if required.
5. Determine the final pixel colors.
6. Apply transparency and priority rules.
7. Draw the visible sprite pixels over the background and window.

This process repeats for every scanline until the entire frame has been rendered.

---

# Relationship with the Background

The Game Boy constructs the final image by combining multiple layers.

Rendering generally follows this order:

```
Background

↓

Window

↓

Sprites

↓

Display
```

The final image shown on the LCD is produced by merging these layers while respecting transparency and priority rules.

---

# Key Takeaways

- Sprites represent movable graphical objects such as characters, enemies, and items.
- Sprite information is stored in Object Attribute Memory (OAM).
- Every sprite is described using four bytes: position, tile number, and attributes.
- Sprites may be either 8×8 or 8×16 pixels in size.
- Transparent pixels allow sprites to blend naturally with the background.
- Priority rules determine whether sprites appear in front of or behind other graphics.
- Horizontal and vertical flipping reduce memory usage by allowing graphics to be reused.
- Although forty sprites can exist in memory, only ten can be rendered on a single scanline.