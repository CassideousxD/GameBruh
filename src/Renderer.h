#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

class Renderer
{
private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    static constexpr int scale = 4;
    static constexpr int screenWidth = 160;
    static constexpr int screenHeight = 144;
    uint32_t pixels[160 * 144];

public:

    Renderer();
    ~Renderer();
    void drawFrame(const uint8_t* framebuffer);
};