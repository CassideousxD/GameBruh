#pragma once
#include "CartridgeHeader.h"
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Cartridge
{
public:
    bool loadROM(const std::string& path);
    size_t getROMSize() const;

    virtual uint8_t  readByte(uint16_t address) const = 0;
    virtual void     writeByte(uint16_t address, uint8_t value) = 0;

    const CartridgeHeader& getHeader() const;

    virtual void saveBattery() {}
    virtual void loadBattery() {}

    virtual ~Cartridge() = default;

    static std::unique_ptr<Cartridge> create(const std::string& path);

protected:
    std::vector<uint8_t> romData;
    std::vector<uint8_t> ramData;
    CartridgeHeader header;
    std::string romPath;

    void parseHeader();
    void allocateRAM();
};