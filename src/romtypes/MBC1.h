#pragma once
#include "../Cartridge.h"

class MBC1 : public Cartridge
{
public:
    uint8_t readByte(uint16_t address) const override;
    void    writeByte(uint16_t address, uint8_t value) override;
    void    saveBattery() override;
    void    loadBattery() override;

private:
    uint8_t romBank    = 1;
    uint8_t ramBank    = 0;
    bool    ramEnabled = false;
    bool    bankingMode = false; // false=ROM, true=RAM
};