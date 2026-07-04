#include "Gameboy.h"
#include <iostream>

GameBoy::GameBoy() : memory(), cpu(memory), timer(memory), apu(memory), ppu(memory), debugger(cpu, memory), inputManager(memory), running(true)
{
    cartridge = Cartridge::create("../ROM/Tetris (World) (Rev 1).gb");
    if(!cartridge)
    {
        std::cerr << "Failed to load ROM!\n";
        running = false;
        return;
    }
    memory.connectCartridge(cartridge.get());
    memory.connectAPU(&apu);
    debugger.disableTracing();
}

void GameBoy::setSpeed(double multiplier)
{
    speedMultiplier = multiplier;
    unlimitedSpeed  = false;

    // mute audio on non-normal speeds to avoid distortion
    // (audio pitch would be wrong at non-1x speeds)
    std::string title = "GameBruh";
    if(multiplier == 0.5) title += " [0.5x]";
    else if(multiplier == 1.0) title += " [1x]";
    else if(multiplier == 2.0) title += " [2x]";
    else if(multiplier == 4.0) title += " [4x]";
    renderer.setTitle(title);
}

void GameBoy::toggleUnlimitedSpeed()
{
    unlimitedSpeed = !unlimitedSpeed;
    renderer.setTitle(unlimitedSpeed ? "GameBruh [MAX]" : "GameBruh [1x]");
    if(unlimitedSpeed)
        speedMultiplier = 1.0;
}

void GameBoy::run()
{
    SDL_Event event;
    const double BASE_MS_PER_FRAME = 1000.0 / 59.73;

    while(running)
    {
        uint64_t frameStartTicks = SDL_GetTicks();

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                running = false;

            inputManager.handleEvent(event);

            if(event.type == SDL_EVENT_KEY_DOWN)
            {
                switch(event.key.key)
                {
                    // Fullscreen
                    case SDLK_F:
                        renderer.toggleFullscreen();
                        break;

                    // Scale
                    case SDLK_1: renderer.setScale(1); break;
                    case SDLK_2: renderer.setScale(2); break;
                    case SDLK_3: renderer.setScale(3); break;
                    case SDLK_4: renderer.setScale(4); break;
                    case SDLK_5: renderer.setScale(5); break;

                    // Palettes
                    case SDLK_F1: renderer.setPalette(Palette::DMG);       break;
                    case SDLK_F2: renderer.setPalette(Palette::Grayscale);  break;
                    case SDLK_F3: renderer.setPalette(Palette::Pocket);     break;
                    case SDLK_F4: renderer.setPalette(Palette::Light);      break;

                    // Scale modes
                    case SDLK_F5: renderer.setScaleMode(ScaleMode::Integer);      break;
                    case SDLK_F6: renderer.setScaleMode(ScaleMode::PixelPerfect); break;
                    case SDLK_F7: renderer.setScaleMode(ScaleMode::Stretch);      break;

                    // Speed control
                    case SDLK_F8:  setSpeed(0.5); break;  // slow mo
                    case SDLK_F9:  setSpeed(1.0); break;  // normal
                    case SDLK_F10: setSpeed(2.0); break;  // 2x
                    case SDLK_F12: setSpeed(4.0); break;  // 4x
                    case SDLK_TAB: toggleUnlimitedSpeed(); break; // unlimited

                    default: break;
                }
            }
        }

        inputManager.update();

        // run extra frames for fast forward
        // e.g. 2x = run 2 frames worth of cycles
        uint32_t framesToRun = 1;
        if(!unlimitedSpeed && speedMultiplier > 1.0)
            framesToRun = static_cast<uint32_t>(speedMultiplier);

        for(uint32_t f = 0; f < framesToRun; f++)
        {
            uint32_t frameCycles = 0;
            while(frameCycles < 70224)
            {
                uint8_t cycles = cpu.executeOpcode();
                timer.update(cycles);
                ppu.update(cycles);

                // only update audio on normal speed to avoid distortion
                if(speedMultiplier <= 1.0)
                    apu.update(cycles);

                frameCycles += cycles;
            }
        }

        if(ppu.isFrameReady())
        {
            renderer.drawFrame(ppu.getFramebuffer());
            ppu.clearFrameReady();
        }

        // frame timing
        if(unlimitedSpeed)
        {
        }
        else
        {
            double msPerFrame = BASE_MS_PER_FRAME / speedMultiplier;
            uint64_t frameTime = SDL_GetTicks() - frameStartTicks;
            if(frameTime < msPerFrame)
                SDL_Delay(static_cast<uint32_t>(msPerFrame - frameTime));
        }
    }
}