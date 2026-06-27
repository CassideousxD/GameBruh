#include "MBC5.h"
#include <fstream>
#include <iostream>

uint8_t MBC5::readByte(uint16_t address) const
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

    if(address >= 0xA000 && address <= 0xBFFF)
    {
        if(!ramEnabled || ramData.empty()) return 0xFF;
        size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
        if(offset < ramData.size())
            return ramData[offset];
        return 0xFF;
    }

    return 0xFF;
}

void MBC5::writeByte(uint16_t address, uint8_t value)
{
    if(address <= 0x1FFF)
    {
        ramEnabled = (value & 0x0F) == 0x0A;
        return;
    }

    // ROM bank low 8 bits
    if(address <= 0x2FFF)
    {
        romBank = (romBank & 0x100) | value;
        return;
    }

    // ROM bank bit 9
    if(address <= 0x3FFF)
    {
        romBank = (romBank & 0xFF) | ((value & 0x01) << 8);
        return;
    }

    // RAM bank
    if(address <= 0x5FFF)
    {
        ramBank = value & 0x0F;
        return;
    }

    // RAM write
    if(address >= 0xA000 && address <= 0xBFFF)
    {
        if(!ramEnabled || ramData.empty()) return;
        size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
        if(offset < ramData.size())
            ramData[offset] = value;
        return;
    }
}

void MBC5::saveBattery()
{
    if(ramData.empty()) return;
    std::string savePath = romPath + ".sav";
    std::ofstream f(savePath, std::ios::binary);
    if(f)
    {
        f.write(reinterpret_cast<const char*>(ramData.data()), ramData.size());
        std::cout << "Battery saved: " << savePath << "\n";
    }
}

void MBC5::loadBattery()
{
    if(ramData.empty()) return;
    std::string savePath = romPath + ".sav";
    std::ifstream f(savePath, std::ios::binary);
    if(f)
    {
        f.read(reinterpret_cast<char*>(ramData.data()), ramData.size());
        std::cout << "Battery loaded: " << savePath << "\n";
    }
}