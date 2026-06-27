#include "Gameboy.h"
#include <iostream>

GameBoy::GameBoy() : cpu(memory), timer(memory), ppu(memory),debugger(cpu, memory),inputManager(memory), running(true)
{
    memory.connectCartridge(&cartridge);
    if(!cartridge.loadROM("../ROM/Dr. Mario (World).gb"))
    {
        std::cerr << "Failed to load ROM!" << std::endl;
        running = false;
    }
    debugger.disableTracing();
}

void GameBoy::run()
{
    SDL_Event event;
    const double MS_PER_FRAME = 1000.0 / 59.73;

    while (running)
    {
        uint64_t frameStartTicks = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Update keyboard ONCE
        inputManager.update();

        uint32_t frameCycles = 0;

        while (frameCycles < 70224)
        {
            uint8_t cycles = cpu.executeOpcode();

            timer.update(cycles);
            ppu.update(cycles);

            frameCycles += cycles;
        }

        if (ppu.isFrameReady())
        {
            renderer.drawFrame(ppu.getFramebuffer());
            ppu.clearFrameReady();
        }

        uint64_t frameTime = SDL_GetTicks() - frameStartTicks;

        if (frameTime < MS_PER_FRAME)
        {
            SDL_Delay(static_cast<uint32_t>(MS_PER_FRAME - frameTime));
        }
    }
}