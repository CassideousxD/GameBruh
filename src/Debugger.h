#pragma once

#include "CPU.h"
#include "Memory.h"

class Debugger
{
private:

    CPU& cpu;
    Memory& memory;
    bool tracingEnabled;

public:

    Debugger( CPU& cpu, Memory& memory);
    void enableTracing();
    void disableTracing();
    bool isTracingEnabled() const;
    void printCPUState() const;
    void printMemory( uint16_t address, uint16_t length) const;
};