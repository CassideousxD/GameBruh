#include "ROMOnly.h"

uint8_t ROMOnly::readByte(uint16_t address) const
{
    if(address < romData.size())
        return romData[address];
    return 0xFF;
}

void ROMOnly::writeByte(uint16_t address, uint8_t value)
{
    // no writes on ROM ONLY
}