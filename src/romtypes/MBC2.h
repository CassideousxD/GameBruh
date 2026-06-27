#pragma once
#include "../Cartridge.h"

class MBC2 : public Cartridge
{
public:
    MBC2();
    uint8_t readByte(uint16_t address) const override;
    void    writeByte(uint16_t address, uint8_t value) override;
    void    saveBattery() override;
    void    loadBattery() override;

private:
    uint8_t romBank    = 1;
    bool    ramEnabled = false;
    // MBC2 has 512x4bit internal RAM
    uint8_t internalRAM[512] = {};
};