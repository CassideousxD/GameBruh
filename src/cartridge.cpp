#include "Cartridge.h"
#include "romtypes/ROMOnly.h"
#include "romtypes/MBC1.h"
#include "romtypes/MBC2.h"
#include "romtypes/MBC3.h"
#include "romtypes/MBC5.h"
#include <fstream>
#include <iostream>

bool Cartridge::loadROM(const std::string& path)
{
    romPath = path;
    std::ifstream romFile(path, std::ios::binary);

    if(!romFile)
    {
        std::cerr << "Failed to open ROM: " << path << "\n";
        return false;
    }

    romFile.seekg(0, std::ios::end);
    std::streamsize romSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    romData.resize(romSize);
    romFile.read(reinterpret_cast<char*>(romData.data()), romSize);

    std::cout << "ROM loaded successfully\n";
    std::cout << "ROM size: " << romSize << " bytes\n";

    parseHeader();
    allocateRAM();
    loadBattery();

    return true;
}

size_t Cartridge::getROMSize() const
{
    return romData.size();
}

const CartridgeHeader& Cartridge::getHeader() const
{
    return header;
}

void Cartridge::allocateRAM()
{
    uint8_t ramSizeCode = romData[0x0149];
    size_t ramSize = 0;

    switch(ramSizeCode)
    {
        case 0x01: ramSize = 2   * 1024; break;  // 2KB (MBC2 internal)
        case 0x02: ramSize = 8   * 1024; break;  // 8KB
        case 0x03: ramSize = 32  * 1024; break;  // 32KB
        case 0x04: ramSize = 128 * 1024; break;  // 128KB
        case 0x05: ramSize = 64  * 1024; break;  // 64KB
        default:   ramSize = 0;          break;
    }

    if(ramSize > 0)
        ramData.resize(ramSize, 0xFF);
}

void Cartridge::parseHeader()
{
    header.title.clear();
    for(size_t i = 0x0134; i <= 0x0143; i++)
    {
        uint8_t c = romData[i];
        if(c == 0x00) break;
        header.title += static_cast<char>(c);
    }

    uint8_t type = romData[0x0147];
    switch(type)
    {
        case 0x00: header.cartridgeType = "ROM ONLY"; break;
        case 0x01: header.cartridgeType = "MBC1"; break;
        case 0x02: header.cartridgeType = "MBC1+RAM"; break;
        case 0x03: header.cartridgeType = "MBC1+RAM+BATTERY"; break;
        case 0x05: header.cartridgeType = "MBC2"; break;
        case 0x06: header.cartridgeType = "MBC2+BATTERY"; break;
        case 0x0F: header.cartridgeType = "MBC3+TIMER+BATTERY"; break;
        case 0x10: header.cartridgeType = "MBC3+TIMER+RAM+BATTERY"; break;
        case 0x11: header.cartridgeType = "MBC3"; break;
        case 0x12: header.cartridgeType = "MBC3+RAM"; break;
        case 0x13: header.cartridgeType = "MBC3+RAM+BATTERY"; break;
        case 0x19: header.cartridgeType = "MBC5"; break;
        case 0x1A: header.cartridgeType = "MBC5+RAM"; break;
        case 0x1B: header.cartridgeType = "MBC5+RAM+BATTERY"; break;
        case 0x1C: header.cartridgeType = "MBC5+RUMBLE"; break;
        case 0x1D: header.cartridgeType = "MBC5+RUMBLE+RAM"; break;
        case 0x1E: header.cartridgeType = "MBC5+RUMBLE+RAM+BATTERY"; break;
        default:   header.cartridgeType = "UNKNOWN"; break;
    }

    uint8_t romSizeCode = romData[0x0148];
    switch(romSizeCode)
    {
        case 0x00: header.romSize = "32 KB";  break;
        case 0x01: header.romSize = "64 KB";  break;
        case 0x02: header.romSize = "128 KB"; break;
        case 0x03: header.romSize = "256 KB"; break;
        case 0x04: header.romSize = "512 KB"; break;
        case 0x05: header.romSize = "1 MB";   break;
        case 0x06: header.romSize = "2 MB";   break;
        case 0x07: header.romSize = "4 MB";   break;
        case 0x08: header.romSize = "8 MB";   break;
        default:   header.romSize = "UNKNOWN";break;
    }

    uint8_t ramSizeCode = romData[0x0149];
    switch(ramSizeCode)
    {
        case 0x00: header.ramSize = "No RAM";  break;
        case 0x01: header.ramSize = "2 KB";    break;
        case 0x02: header.ramSize = "8 KB";    break;
        case 0x03: header.ramSize = "32 KB";   break;
        case 0x04: header.ramSize = "128 KB";  break;
        case 0x05: header.ramSize = "64 KB";   break;
        default:   header.ramSize = "UNKNOWN"; break;
    }

    uint8_t ver = romData[0x014C];
    header.version = "Version " + std::to_string(ver);
    header.headerChecksum = romData[0x014D];
    header.globalChecksum = (romData[0x014E] << 8) | romData[0x014F];

    std::cout << "Title: "    << header.title         << "\n";
    std::cout << "Type: "     << header.cartridgeType << "\n";
    std::cout << "ROM Size: " << header.romSize       << "\n";
    std::cout << "RAM Size: " << header.ramSize       << "\n";
}

std::unique_ptr<Cartridge> Cartridge::create(const std::string& path)
{
    // peek at type byte before full load
    std::ifstream f(path, std::ios::binary);
    if(!f) return nullptr;

    f.seekg(0x0147);
    uint8_t type = 0;
    f.read(reinterpret_cast<char*>(&type), 1);

    std::unique_ptr<Cartridge> cart;

    switch(type)
    {
        case 0x00:
            cart = std::make_unique<ROMOnly>();
            break;

        case 0x01:
        case 0x02:
        case 0x03:
            cart = std::make_unique<MBC1>();
            break;

        case 0x05:
        case 0x06:
            cart = std::make_unique<MBC2>();
            break;

        case 0x0F:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            cart = std::make_unique<MBC3>();
            break;

        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
            cart = std::make_unique<MBC5>();
            break;

        default:
            std::cerr << "Unknown cartridge type: 0x"
                      << std::hex << (int)type << "\n";
            cart = std::make_unique<ROMOnly>();
            break;
    }

    if(!cart->loadROM(path))
        return nullptr;

    return cart;
}