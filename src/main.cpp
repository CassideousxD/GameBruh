#include "Gameboy.h"
#include <SDL3/SDL.h>
#include <iostream>

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    {
        GameBoy gameBoy;  
        gameBoy.run();
    }

    SDL_Quit();
    return 0;
}