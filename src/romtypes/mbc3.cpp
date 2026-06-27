#include "MBC3.h"
#include <fstream>
#include <iostream>
#include <ctime>

uint8_t MBC3::readByte(uint16_t address) const
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
        if(!ramEnabled) return 0xFF;

        if(ramBank <= 0x03)
        {
            // RAM read
            size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
            if(offset < ramData.size())
                return ramData[offset];
            return 0xFF;
        }

        // RTC register read
        switch(ramBank)
        {
            case 0x08: return latchedRTC.seconds;
            case 0x09: return latchedRTC.minutes;
            case 0x0A: return latchedRTC.hours;
            case 0x0B: return latchedRTC.daysLow;
            case 0x0C: return latchedRTC.daysHigh;
        }
    }

    return 0xFF;
}

void MBC3::writeByte(uint16_t address, uint8_t value)
{
    if(address <= 0x1FFF)
    {
        ramEnabled = (value & 0x0F) == 0x0A;
        return;
    }

    if(address <= 0x3FFF)
    {
        romBank = value & 0x7F;
        if(romBank == 0) romBank = 1;
        return;
    }

    if(address <= 0x5FFF)
    {
        ramBank = value;
        return;
    }

    if(address <= 0x7FFF)
    {
        // RTC latch - write 0x00 then 0x01 to latch
        if(latchWritten == 0x00 && value == 0x01)
            latchRTC();
        latchWritten = value;
        return;
    }

    if(address >= 0xA000 && address <= 0xBFFF)
    {
        if(!ramEnabled) return;

        if(ramBank <= 0x03)
        {
            size_t offset = (size_t)ramBank * 0x2000 + (address - 0xA000);
            if(offset < ramData.size())
                ramData[offset] = value;
            return;
        }

        // RTC register write
        switch(ramBank)
        {
            case 0x08: rtc.seconds  = value & 0x3F; break;
            case 0x09: rtc.minutes  = value & 0x3F; break;
            case 0x0A: rtc.hours    = value & 0x1F; break;
            case 0x0B: rtc.daysLow  = value;        break;
            case 0x0C: rtc.daysHigh = value & 0xC1; break;
        }
        return;
    }
}

void MBC3::latchRTC()
{
    // tick RTC from system time
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    rtc.seconds  = t->tm_sec;
    rtc.minutes  = t->tm_min;
    rtc.hours    = t->tm_hour;
    uint16_t days = t->tm_yday;
    rtc.daysLow  = days & 0xFF;
    rtc.daysHigh = (days >> 8) & 0x01;

    latchedRTC = rtc;
}

void MBC3::saveBattery()
{
    if(ramData.empty()) return;
    std::string savePath = romPath + ".sav";
    std::ofstream f(savePath, std::ios::binary);
    if(f)
    {
        f.write(reinterpret_cast<const char*>(ramData.data()), ramData.size());
        // also save RTC
        f.write(reinterpret_cast<const char*>(&rtc), sizeof(rtc));
        std::cout << "Battery saved: " << savePath << "\n";
    }
}

void MBC3::loadBattery()
{
    if(ramData.empty()) return;
    std::string savePath = romPath + ".sav";
    std::ifstream f(savePath, std::ios::binary);
    if(f)
    {
        f.read(reinterpret_cast<char*>(ramData.data()), ramData.size());
        f.read(reinterpret_cast<char*>(&rtc), sizeof(rtc));
        latchedRTC = rtc;
        std::cout << "Battery loaded: " << savePath << "\n";
    }
}