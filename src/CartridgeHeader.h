#pragma once
#include <cstdint>
#include <string>

struct CartridgeHeader
{
    std::string title;
    std::string cartridgeType;
    std::string romSize;
    std::string ramSize;
    std::string version;
    uint8_t headerChecksum;
    uint16_t globalChecksum;
};