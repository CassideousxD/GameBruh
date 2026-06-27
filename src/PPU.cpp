#include "PPU.h"
#include <iostream>

PPU::PPU(Memory& memory) : memory(memory)
{
    for(int i = 0; i < 160 * 144; i++)
    {
        framebuffer[i] = 0;
    }
    ppuCycles = 0;
    setLCDMode(2);
}

void PPU::update(uint16_t cycles)
{
    if (!isLCDEnabled())
    {
        ppuCycles = 0;
        memory.setLY(0);
        updateLYCompare();
        setLCDMode(0);
        statInterruptActive = false;
        return;
    }
    ppuCycles += cycles;
    if (ppuCycles >= 456)
    {
        ppuCycles -= 456;
        uint8_t ly = memory.getLY();
        ly = (ly + 1) % 154; 
        memory.setLY(ly);
        updateLYCompare();
        checkSTATInterrupt();
        if (ly == 144)
        {
            setLCDMode(1); 
            checkSTATInterrupt();
            frameReady = true;
            memory.requestVBlankInterrupt();
        }
    }
    uint8_t currentLY = memory.getLY();
    if (currentLY >= 144)
    {
        setLCDMode(1); 
    }
    else
    {
        if (ppuCycles < 80)
        {
            setLCDMode(2);
        }
        else if (ppuCycles < 252)
        {
            setLCDMode(3); 
            if (ppuCycles - cycles < 80) 
            {
                renderBackgroundScanline();
                renderSpritesScanline();
            }
        }
        else
        {
            setLCDMode(0); 
        }
    }
}

void PPU::decodeTile(uint16_t tileAddress)
{
    for(int row = 0; row < 8; row++)
    {
        uint8_t lowByte = memory.readByte( tileAddress + row * 2);
        uint8_t highByte = memory.readByte( tileAddress + row * 2 + 1);

        for(int pixel = 0; pixel < 8; pixel++)
        {
            int bit = 7 - pixel;
            uint8_t lowBit = (lowByte >> bit) & 1;
            uint8_t highBit = (highByte >> bit) & 1;
            uint8_t color = (highBit << 1) | lowBit;
            framebuffer[ row * 8 + pixel ] = color;
        }
    }
}

uint8_t PPU::getFramebufferPixel( uint16_t index) const
{
    return framebuffer[index];
}

bool PPU::isFrameReady() const
{
    return frameReady;
}

void PPU::clearFrameReady()
{
    frameReady = false;
}

uint16_t PPU::getWindowTileMapAddress() const
{
    if(memory.getLCDC() & 0x40)
    {
        return 0x9C00;
    }
    return 0x9800;
}

bool PPU::isWindowVisibleOnCurrentScanline() const
{
    if(!isWindowEnabled())
    {
        return false;
    }

    return memory.getLY() >= memory.getWY();
}

int PPU::getWindowX() const
{
    return static_cast<int>(memory.getWX()) - 7;
}

int PPU::getWindowY() const
{
    return static_cast<int>(memory.getWY());
}

void PPU::drawTileRow(uint16_t tileAddress, int screenX, int screenY, uint8_t currentLY)
{
    if (currentLY < screenY || currentLY >= screenY + 8)
    {
        return;
    }
    int row = currentLY - screenY;
    uint8_t lowByte = memory.readByte(tileAddress + row * 2);
    uint8_t highByte = memory.readByte(tileAddress + row * 2 + 1);
    for (int pixel = 0; pixel < 8; pixel++)
    {
        int bit = 7 - pixel;
        uint8_t lowBit = (lowByte >> bit) & 1;
        uint8_t highBit = (highByte >> bit) & 1;
        uint8_t color = (highBit << 1) | lowBit;
        int finalX = screenX + pixel;
        if (finalX < 0 || finalX >= 160)
        {
            continue;
        }
        framebuffer[currentLY * 160 + finalX] = color;
    }
}

void PPU::renderBackgroundScanline()
{
    uint8_t currentLY = memory.getLY();
    if (currentLY >= 144) return; 

    uint16_t backgroundTileMap = getBackgroundTileMap();
    int scrolledY = currentLY + memory.getSCY();
    int tileY = (scrolledY / 8) & 31;
    int rowInTile = scrolledY % 8;

    for(int tileX = 0; tileX < 21; tileX++)
    {
        int scrolledX = tileX * 8 + memory.getSCX();
        int mapTileX = (scrolledX / 8) & 31;
        uint16_t mapIndex = tileY * 32 + mapTileX;

        uint8_t tileNumber = memory.readByte(backgroundTileMap + mapIndex);
        uint16_t tileAddress = 0x8000;
        if (useSignedTileData())
        {
            int8_t signedTile = static_cast<int8_t>(tileNumber);
            tileAddress = 0x9000 + (signedTile * 16);
        }
        else
        {
            tileAddress = 0x8000 + (tileNumber * 16);
        }

        uint8_t lowByte = memory.readByte(tileAddress + rowInTile * 2);
        uint8_t highByte = memory.readByte(tileAddress + rowInTile * 2 + 1);

        int screenX = tileX * 8 - (memory.getSCX() % 8);

        for(int pixel = 0; pixel < 8; pixel++)
        {
            int finalX = screenX + pixel;
            if(finalX < 0 || finalX >= 160) continue;

            int bit = 7 - pixel;
            uint8_t lowBit = (lowByte >> bit) & 1;
            uint8_t highBit = (highByte >> bit) & 1;

            framebuffer[currentLY * 160 + finalX] = (highBit << 1) | lowBit;
        }
    }

    if(!isWindowVisibleOnCurrentScanline()) return;

    uint16_t windowTileMap = getWindowTileMapAddress();
    int windowLine = currentLY - getWindowY();
    int windowTileY = windowLine / 8;
    int windowRow = windowLine % 8;

    for(int tileX = 0; tileX < 21; tileX++)
    {
        uint16_t mapIndex = windowTileY * 32 + tileX;
        uint8_t tileNumber = memory.readByte(windowTileMap + mapIndex);
        uint16_t tileAddress = 0x8000;
        if (useSignedTileData())
        {
            int8_t signedTile = static_cast<int8_t>(tileNumber);
            tileAddress = 0x9000 + (signedTile * 16);
        }
        else
        {
            tileAddress = 0x8000 + (tileNumber * 16);
        }

        uint8_t lowByte = memory.readByte(tileAddress + windowRow * 2);
        uint8_t highByte = memory.readByte(tileAddress + windowRow * 2 + 1);

        int screenX = getWindowX() + tileX * 8;

        for(int pixel = 0; pixel < 8; pixel++)
        {
            int finalX = screenX + pixel;
            if(finalX < 0 || finalX >= 160) continue;

            int bit = 7 - pixel;
            uint8_t lowBit = (lowByte >> bit) & 1;
            uint8_t highBit = (highByte >> bit) & 1;

            framebuffer[currentLY * 160 + finalX] = (highBit << 1) | lowBit;
        }
    }
}

void PPU::renderSpritesScanline()
{
    if (!isSpritesEnabled())
    {
        return;
    }
    uint8_t currentLY = memory.getLY();
    uint8_t spriteHeight = getSpriteHeight(); 
    for (int sprite = 0; sprite < 40; sprite++)
    {
        uint16_t spriteAddress = 0xFE00 + (sprite * 4);
        int spriteY = static_cast<int>(memory.readByte(spriteAddress)) - 16;
        int spriteX = static_cast<int>(memory.readByte(spriteAddress + 1)) - 8;
        uint8_t tileNumber = memory.readByte(spriteAddress + 2);
        uint8_t attributes = memory.readByte(spriteAddress + 3);
        if (currentLY < spriteY || currentLY >= (spriteY + spriteHeight))
        {
            continue; 
        }
        int row = currentLY - spriteY;
        if (attributes & 0x40)
        {
            row = spriteHeight - 1 - row;
        }
        uint16_t tileAddress = 0x8000 + (tileNumber * 16);
        uint8_t lowByte = memory.readByte(tileAddress + row * 2);
        uint8_t highByte = memory.readByte(tileAddress + row * 2 + 1);
        for (int pixel = 0; pixel < 8; pixel++)
        {
            int finalX = spriteX + pixel;
            if (finalX < 0 || finalX >= 160)
            {
                continue;
            }
            int bit = 7 - pixel;
            if (attributes & 0x20)
            {
                bit = pixel; 
            }
            uint8_t lowBit = (lowByte >> bit) & 1;
            uint8_t highBit = (highByte >> bit) & 1;
            uint8_t colorCode = (highBit << 1) | lowBit;
            if (colorCode == 0)
            {
                continue; 
            }
            framebuffer[currentLY * 160 + finalX] = colorCode;
        }
    }
}

const uint8_t* PPU::getFramebuffer() const
{
    return framebuffer;
}

bool PPU::isLCDEnabled() const
{
    return memory.readByte(0xFF40) & 0x80;
}


bool PPU::isSpritesEnabled() const
{
    return memory.readByte(0xFF40) & 0x02;
}

bool PPU::isBackgroundEnabled() const
{
    return memory.readByte(0xFF40) & 0x01;
}

bool PPU::useSignedTileData() const
{
    return !(memory.readByte(0xFF40) & 0x10);
}

uint16_t PPU::getBackgroundTileMap() const
{
    if (memory.readByte(0xFF40) & 0x08)
    {
        return 0x9C00;
    }

    return 0x9800;
}

uint16_t PPU::getWindowTileMap() const
{
    if (memory.readByte(0xFF40) & 0x40)
    {
        return 0x9C00;
    }

    return 0x9800;
}

uint8_t PPU::getSpriteHeight() const
{
    if (memory.readByte(0xFF40) & 0x04)
    {
        return 16;
    }

    return 8;
}

uint8_t PPU::getLCDMode() const
{
    return memory.readByte(0xFF41) & 0x03;
}

bool PPU::isWindowEnabled() const
{
    return (memory.getLCDC() & 0x20) != 0;
}

void PPU::setLCDMode(uint8_t mode)
{
    uint8_t stat = memory.readByte(0xFF41);

    stat &= 0xFC;

    stat |= (mode & 0x03);

    memory.writeByte(0xFF41, stat);
}

bool PPU::isLYCEqual() const
{
    return memory.readByte(0xFF41) & 0x04;
}

void PPU::setLYCEqual(bool equal)
{
    uint8_t stat = memory.readByte(0xFF41);

    if (equal)
    {
        stat |= 0x04;
    }
    else
    {
        stat &= ~0x04;
    }

    memory.writeByte(0xFF41, stat);
}

void PPU::updateLYCompare()
{
    uint8_t ly = memory.getLY();
    uint8_t lyc = memory.readByte(0xFF45);

    setLYCEqual(ly == lyc);
}

void PPU::checkSTATInterrupt()
{
    uint8_t stat = memory.readByte(0xFF41);
    uint8_t mode = stat & 0x03;

    bool interruptRequested = false;

    switch (mode)
    {
        case 0:
            interruptRequested = stat & 0x08;
            break;

        case 1:
            interruptRequested = stat & 0x10;
            break;

        case 2:
            interruptRequested = stat & 0x20;
            break;
    }

    if ((stat & 0x40) && isLYCEqual())
    {
        interruptRequested = true;
    }

    if (interruptRequested && !statInterruptActive)
    {
        memory.requestSTATInterrupt();
    }

    statInterruptActive = interruptRequested;
}

