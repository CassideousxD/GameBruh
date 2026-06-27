#include "Debugger.h"
#include <iostream>
#include <iomanip>

Debugger::Debugger(
    CPU& cpu,
    Memory& memory)
    : cpu(cpu),
      memory(memory),
      tracingEnabled(false)
{
}

void Debugger::enableTracing()
{
    tracingEnabled = true;
}

void Debugger::disableTracing()
{
    tracingEnabled = false;
}

bool Debugger::isTracingEnabled() const
{
    return tracingEnabled;
}

void Debugger::printMemory(
    uint16_t address,
    uint16_t length) const
{
    std::cout
        << std::hex
        << std::uppercase
        << std::setfill('0');

    for(uint16_t i = 0; i < length; i++)
    {
        if(i % 16 == 0)
        {
            std::cout
                << "\n"
                << std::setw(4)
                << address + i
                << ": ";
        }

        std::cout
            << std::setw(2)
            << (int)memory.readByte(address + i)
            << ' ';
    }
    std::cout << '\n';
}

void Debugger::printCPUState() const
{
    uint16_t pc = cpu.getProgramCounter();

    std::cout
        << std::uppercase
        << std::hex
        << std::setfill('0');

    std::cout
        << "A:"  << std::setw(2) << (int)cpu.getA() << " "
        << "F:"  << std::setw(2) << (int)cpu.getF() << " "
        << "B:"  << std::setw(2) << (int)cpu.getB() << " "
        << "C:"  << std::setw(2) << (int)cpu.getC() << " "
        << "D:"  << std::setw(2) << (int)cpu.getD() << " "
        << "E:"  << std::setw(2) << (int)cpu.getE() << " "
        << "H:"  << std::setw(2) << (int)cpu.getH() << " "
        << "L:"  << std::setw(2) << (int)cpu.getL() << " "

        << "SP:"
        << std::setw(4)
        << cpu.getStackPointer()
        << " "

        << "PC:"
        << std::setw(4)
        << pc
        << " "

        << "PCMEM:"
        << std::setw(2)
        << (int)memory.readByte(pc)
        << ","

        << std::setw(2)
        << (int)memory.readByte(pc + 1)
        << ","

        << std::setw(2)
        << (int)memory.readByte(pc + 2)
        << ","

        << std::setw(2)
        << (int)memory.readByte(pc + 3)

        << '\n';
}