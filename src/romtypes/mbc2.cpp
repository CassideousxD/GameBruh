#include "MBC2.h"
#include <fstream>
#include <iostream>
#include <cstring>

MBC2::MBC2()
{
    std::memset(internalRAM, 0xFF, sizeof(internalRAM));
}

uint8_t MBC2::readByte(uint16_t address) const
{
    if(address <= 0x3FFF)
        return romData[address];

    if(address <= 0x7FFF)
    {
        size_t offset = (size_t)romBank * 0x4000 + (address - 0x4000);
        if(offset < romData.size())
            return romData[offset];
        return 0xFF;
    }

    // MBC2 RAM - 512x4bit mapped at A000-A1FF
    if(address >= 0xA000 && address <= 0xA1FF)
    {
        if(!ramEnabled) return 0xFF;
        return internalRAM[address - 0xA000] | 0xF0; // upper nibble always 1
    }

    return 0xFF;
}

void MBC2::writeByte(uint16_t address, uint8_t value)
{
    if(address <= 0x3FFF)
    {
        // bit 8 of address determines RAM enable vs ROM bank
        if(address & 0x0100)
        {
            // ROM bank select
            romBank = value & 0x0F;
            if(romBank == 0) romBank = 1;
        }
        else
        {
            // RAM enable
            ramEnabled = (value & 0x0F) == 0x0A;
        }
        return;
    }

    if(address >= 0xA000 && address <= 0xA1FF)
    {
        if(!ramEnabled) return;
        internalRAM[address - 0xA000] = value & 0x0F; // only lower nibble
        return;
    }
}

void MBC2::saveBattery()
{
    std::string savePath = romPath + ".sav";
    std::ofstream f(savePath, std::ios::binary);
    if(f)
    {
        f.write(reinterpret_cast<const char*>(internalRAM), sizeof(internalRAM));
        std::cout << "Battery saved: " << savePath << "\n";
    }
}

void MBC2::loadBattery()
{
    std::string savePath = romPath + ".sav";
    std::ifstream f(savePath, std::ios::binary);
    if(f)
    {
        f.read(reinterpret_cast<char*>(internalRAM), sizeof(internalRAM));
        std::cout << "Battery loaded: " << savePath << "\n";
    }
}