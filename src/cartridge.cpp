#include "Cartridge.h"
#include <fstream>
#include <iostream>

bool Cartridge::loadROM(const std::string& path)
{
    std::ifstream romFile(path, std::ios::binary);

    if (!romFile)
    {
        std::cout << "Failed to open ROM: "
                  << path
                  << std::endl;

        return false;
    }

    romFile.seekg(0, std::ios::end);
    std::streamsize romSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    romData.resize(romSize);

    romFile.read(reinterpret_cast<char*>(romData.data()),romSize);

    std::cout << "ROM loaded successfully" << std::endl;
    std::cout << "ROM size: "
              << romSize
              << " bytes"
              << std::endl;

    parseHeader();

    return true;
}

size_t Cartridge::getROMSize() const
{
    return romData.size();
}

uint8_t Cartridge::readByte(size_t address) const
{
    if(address >= romData.size())
    {
        return 0xFF;
    }

    return romData[address];
}

const CartridgeHeader& Cartridge::getHeader() const
{
    return header;
}

void Cartridge::parseHeader()
{

    header.title.clear();
    header.cartridgeType.clear();
    header.romSize.clear();
    header.ramSize.clear();
    header.version.clear();

   for (size_t address = 0x0134; address <= 0x0143; address++)
    {
        uint8_t value = readByte(address);

        if (value == 0x00)
        {
            break;
        }

        header.title += static_cast<char>(value);
    }

    uint8_t cartridgeType = readByte(0x0147);
    switch (cartridgeType)
    {
        case 0x00:
            header.cartridgeType = "ROM ONLY";
            break;

        case 0x01:
            header.cartridgeType = "MBC1";
            break;

        case 0x02:
            header.cartridgeType = "MBC1 + RAM";
            break;

        case 0x03:
            header.cartridgeType = "MBC1 + RAM + BATTERY";
            break;

        default:
            header.cartridgeType = "UNKNOWN";
            break;
    }

    uint8_t romSizeCode = readByte(0x0148);
    switch (romSizeCode)

    {
        case 0x00:
            header.romSize = "32 KB";
            break;

        case 0x01:
            header.romSize = "64 KB";
            break;

        case 0x02:
            header.romSize = "128 KB";
            break;

        case 0x03:
            header.romSize = "256 KB";
            break;

        case 0x04:
            header.romSize = "512 KB";
            break;

        case 0x05:
            header.romSize = "1 MB";
            break;

        case 0x06:
            header.romSize = "2 MB";
            break;

        case 0x07:
            header.romSize = "4 MB";
            break;

        case 0x08:
            header.romSize = "8 MB";
            break;

        default:
            header.romSize = "UNKNOWN";
            break;
    }

    uint8_t ramSizeCode = readByte(0x0149);
    switch (ramSizeCode)
    {
        case 0x00:
            header.ramSize = "No RAM";
            break;

        case 0x02:
            header.ramSize = "8 KB";
            break;

        case 0x03:
            header.ramSize = "32 KB";
            break;

        case 0x04:
            header.ramSize = "128 KB";
            break;

        case 0x05:
            header.ramSize = "64 KB";
            break;

        default:
            header.ramSize = "UNKNOWN";
            break;
    }

    uint8_t versionNumber = readByte(0x014C);
    header.version = "Version " + std::to_string(versionNumber);

    header.headerChecksum = readByte(0x014D);
    header.globalChecksum =(readByte(0x014E) << 8)| readByte(0x014F);
}