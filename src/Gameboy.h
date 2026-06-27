#pragma once
#include "Memory.h"
#include "CPU.h"
#include "PPU.h"
#include "Timer.h"
#include "Renderer.h"
#include "Debugger.h"
#include "InputManager.h"
#include "APU.h"
#include "Cartridge.h"
#include <memory>

class GameBoy
{
private:
    std::unique_ptr<Cartridge> cartridge; 
    Memory memory;
    CPU cpu;
    Timer timer;
    APU apu;
    PPU ppu;
    Renderer renderer;
    Debugger debugger;
    bool running;
    InputManager inputManager;

public:
    GameBoy();
    void run();
};