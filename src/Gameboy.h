#pragma once

#include "Cartridge.h"
#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Timer.h"
#include "Renderer.h"
#include "Debugger.h"
#include "InputManager.h"

class GameBoy
{
private:

    Cartridge cartridge;
    Memory memory;
    CPU cpu;
    Timer timer;
    PPU ppu;
    Renderer renderer;
    Debugger debugger;
    bool running;
    InputManager inputManager;

public:

    GameBoy();
    void run();
};