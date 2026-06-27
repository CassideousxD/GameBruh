#pragma once
#include "Memory.h"

class Timer
{
private:

    Memory& memory;
    uint16_t divCounter;
    uint16_t timerCounter;
    uint16_t getTimerFrequency();

public:

    Timer(Memory& memory);
    void update(uint16_t cycles);
};