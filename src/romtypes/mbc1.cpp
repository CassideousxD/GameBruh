#include "MBC1.h"
#include <fstream>
#include <iostream>

uint8_t MBC1::readByte(uint16_t address) const
{
    if(address <= 0x3FFF)
    {
        // Bank 0 - fixed
        return romData[address];
    }

    if(address <= 0x7FFF)
    {
        // Switchable ROM bank
        size_t offset = (size_t)romBank * 0x4000 + (address - 0x4000);
        if(offset < romData.size())
            return romData[offset];
        return 0xFF;
    }

    if(address >= 0xA000 && address <= 0xBFFF)
    {
        if(!ramEnabled || ramData.empty())
            return 0xFF;
        size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
        if(offset < ramData.size())
            return ramData[offset];
        return 0xFF;
    }

    return 0xFF;
}

void MBC1::writeByte(uint16_t address, uint8_t value)
{
    // RAM enable
    if(address <= 0x1FFF)
    {
        ramEnabled = (value & 0x0F) == 0x0A;
        return;
    }

    // ROM bank number lower 5 bits
    if(address <= 0x3FFF)
    {
        uint8_t bank = value & 0x1F;
        if(bank == 0) bank = 1;
        romBank = (romBank & 0x60) | bank;

        // mask to valid banks
        uint8_t maxBank = (romData.size() / 0x4000);
        romBank &= (maxBank - 1);
        if(romBank == 0) romBank = 1;
        return;
    }

    // RAM bank / upper ROM bank bits
    if(address <= 0x5FFF)
    {
        uint8_t bits = value & 0x03;
        if(bankingMode)
        {
            ramBank = bits;
        }
        else
        {
            romBank = (romBank & 0x1F) | (bits << 5);
            uint8_t maxBank = (romData.size() / 0x4000);
            romBank &= (maxBank - 1);
            if(romBank == 0) romBank = 1;
        }
        return;
    }

    // Banking mode select
    if(address <= 0x7FFF)
    {
        bankingMode = value & 0x01;
        if(!bankingMode) ramBank = 0;
        return;
    }

    // Write to external RAM
    if(address >= 0xA000 && address <= 0xBFFF)
    {
        if(!ramEnabled || ramData.empty()) return;
        size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
        if(offset < ramData.size())
            ramData[offset] = value;
        return;
    }
}

void MBC1::saveBattery()
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

void MBC1::loadBattery()
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