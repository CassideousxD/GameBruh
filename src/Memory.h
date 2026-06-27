#pragma once
#include "Cartridge.h"
#include "Joypad.h"
#include <cstdint>

class Memory
{
public:
    Memory();
    uint8_t readByte(uint16_t address) const;
    void writeByte(uint16_t address, uint8_t value);
    void incrementDIV();
    void requestTimerInterrupt();
    uint8_t getLY() const;
    void setLY(uint8_t value);
    void requestVBlankInterrupt();
    void requestSTATInterrupt();
    uint8_t getSCX() const;
    uint8_t getSCY() const;
    uint8_t getWX() const;
    uint8_t getWY() const;
    uint8_t getLCDC() const;

    void setSCX(uint8_t value);
    void setSCY(uint8_t value);
    void setWX(uint8_t value);
    void setWY(uint8_t value);

    void connectCartridge(Cartridge* cartridge);
    void initializeHardwareRegisters();
    void doDMATransfer(uint8_t value);
    void pressButton(Button button);
    void releaseButton(Button button);
private:

    uint8_t memory[65536];
    uint8_t interruptEnable;
    uint8_t interruptFlag;
    uint8_t divRegister;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
    uint8_t ly;
    uint8_t lcdc;
    uint8_t scx;
    uint8_t scy;
    uint8_t wx;
    uint8_t wy;
    Cartridge* cartridge;
    Joypad joypad;
    uint8_t serialData;
    uint8_t serialControl;
    uint8_t dma = 0;
    bool bootRomEnabled = true;
    uint8_t bootRom[256];
    uint8_t buttonMatrix[2];    
    uint8_t joypadSelect;
};