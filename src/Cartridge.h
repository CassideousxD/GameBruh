#pragma once
#include "CartridgeHeader.h"

#include <cstdint>
#include <string>
#include <vector>

class Cartridge
{
public:
    bool loadROM(const std::string& path);
    size_t getROMSize() const;
    uint8_t readByte(size_t address) const;
    const CartridgeHeader& getHeader() const;

private:
    std::vector<uint8_t> romData;
    CartridgeHeader header;
    void parseHeader();
};