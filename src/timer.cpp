#include "Timer.h"

Timer::Timer(Memory& memory) : memory(memory)
{
    divCounter = 0;
    timerCounter = 0;
}

void Timer::update(uint16_t cycles)
{
    divCounter += cycles;
    while (divCounter >= 256)
    {
        divCounter -= 256;
        memory.incrementDIV();
    }

    uint8_t tac = memory.readByte(0xFF07);
    if (!(tac & 0x04)) return;  

    timerCounter += cycles;
    uint16_t frequency = getTimerFrequency();
    while (timerCounter >= frequency)
    {
        timerCounter -= frequency;
        uint8_t tima = memory.readByte(0xFF05);
        if (tima == 0xFF)
        {
            memory.writeByte(0xFF05, memory.readByte(0xFF06));
            memory.requestTimerInterrupt();
        }
        else
        {
            memory.writeByte(0xFF05, tima + 1);
        }
    }
}

uint16_t Timer::getTimerFrequency()
{
    uint8_t tac = memory.readByte(0xFF07);
    switch(tac & 0x03)
    {
        case 0x00:
            return 1024;

        case 0x01:
            return 16;

        case 0x02:
            return 64;

        case 0x03:
            return 256;
    }
    return 1024;
}