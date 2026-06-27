#pragma once
#include "../Cartridge.h"

class ROMOnly : public Cartridge
{
public:
    uint8_t readByte(uint16_t address) const override;
    void    writeByte(uint16_t address, uint8_t value) override;
};