#pragma once
#include "../Cartridge.h"
#include <ctime>

class MBC3 : public Cartridge
{
public:
    uint8_t readByte(uint16_t address) const override;
    void    writeByte(uint16_t address, uint8_t value) override;
    void    saveBattery() override;
    void    loadBattery() override;

private:
    uint8_t romBank    = 1;
    uint8_t ramBank    = 0;
    bool    ramEnabled = false;

    // RTC registers
    struct RTC
    {
        uint8_t seconds = 0;
        uint8_t minutes = 0;
        uint8_t hours   = 0;
        uint8_t daysLow = 0;
        uint8_t daysHigh = 0; // bit0=day MSB, bit6=halt, bit7=carry
    };

    RTC rtc;
    RTC latchedRTC;
    bool    rtcLatched   = false;
    uint8_t latchWritten = 0xFF;
    bool    rtcSelected  = false; // true when RAM bank is 0x08-0x0C

    void latchRTC();
    void tickRTC();
};