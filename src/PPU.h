#pragma once
#include "Memory.h"

class PPU
{
private:

    Memory& memory;
    uint8_t framebuffer[160 * 144];
    uint32_t ppuCycles;
    void decodeTile(uint16_t tileAddress);
    void renderBackgroundScanline();
    void renderSpritesScanline();
    void drawTileRow(uint16_t tileAddress, int screenX, int screenY, uint8_t currentLY);
    int modeClock = 0;
    static constexpr uint16_t MODE2_CYCLES = 80;
    static constexpr uint16_t MODE3_CYCLES = 172;
    static constexpr uint16_t MODE0_CYCLES = 204;
    static constexpr uint16_t SCANLINE_CYCLES = 456;
    bool statInterruptActive = false;
    bool frameReady = false;

public:

    PPU(Memory& memory);
    void update(uint16_t cycles);
    uint8_t getFramebufferPixel( uint16_t index) const;
    const uint8_t* getFramebuffer() const;

    bool isLCDEnabled() const;
    bool isWindowEnabled() const;
    bool isSpritesEnabled() const;
    bool isBackgroundEnabled() const;
    bool useSignedTileData() const;
    uint16_t getBackgroundTileMap() const;
    uint16_t getWindowTileMap() const;
    uint8_t getSpriteHeight() const;
    uint8_t getLCDMode() const;
    void setLCDMode(uint8_t mode);
    bool isLYCEqual() const;
    void setLYCEqual(bool equal);
    void updateLYCompare();
    void checkSTATInterrupt();
    bool isFrameReady() const;
    void clearFrameReady();
    uint16_t getWindowTileMapAddress() const;
    bool isWindowVisibleOnCurrentScanline() const;
    int getWindowX() const;
    int getWindowY() const;

};