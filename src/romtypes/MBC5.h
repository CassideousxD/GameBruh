#pragma once
#include "../Cartridge.h"

class MBC5 : public Cartridge
{
public:
    uint8_t readByte(uint16_t address) const override;
    void    writeByte(uint16_t address, uint8_t value) override;
    void    saveBattery() override;
    void    loadBattery() override;

private:
    uint16_t romBank   = 1;  // 9 bits!
    uint8_t  ramBank   = 0;
    bool     ramEnabled = false;
};