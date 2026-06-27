#include "Renderer.h"

Renderer::Renderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow( "Game Boy Emulator", 160 * scale, 144 * scale, 0);
    renderer = SDL_CreateRenderer( window, nullptr);
    if (renderer) 
    {
        SDL_SetRenderVSync(renderer, 1); 
    }
    texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
}

Renderer::~Renderer()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::drawFrame(const uint8_t* framebuffer)
{
    for(int i = 0; i < 160 * 144; i++)
    {
        switch(framebuffer[i])
        {
            case 0:
                pixels[i] = 0xFFFFFFFF;
                break;

            case 1:
                pixels[i] = 0xAAAAAAFF;
                break;

            case 2:
                pixels[i] = 0x555555FF;
                break;

            case 3:
                pixels[i] = 0x000000FF;
                break;
        }
    }

    SDL_UpdateTexture( texture, nullptr, pixels, 160 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture( renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}