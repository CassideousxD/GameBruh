#include "Renderer.h"
#include <iostream>
#include <algorithm>

Renderer::Renderer()
{
    window = SDL_CreateWindow(
        "GameBruh",
        screenWidth  * currentScale,
        screenHeight * currentScale,
        SDL_WINDOW_RESIZABLE);

    if(!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        return;
    }

    SDL_SetRenderVSync(renderer, 1);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth,
        screenHeight);

    if(!texture)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
        return;
    }
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    applyPalette(currentPalette);
}

Renderer::~Renderer()
{
    if(texture)  SDL_DestroyTexture(texture);
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window)   SDL_DestroyWindow(window);
}

void Renderer::applyPalette(Palette p)
{
    currentPalette = p;
    switch(p)
    {
        case Palette::DMG:
            // original GB green tones
            paletteColors[0] = 0x9BBC0FFF;
            paletteColors[1] = 0x8BAC0FFF;
            paletteColors[2] = 0x306230FF;
            paletteColors[3] = 0x0F380FFF;
            break;

        case Palette::Grayscale:
            paletteColors[0] = 0xFFFFFFFF;
            paletteColors[1] = 0xAAAAAAFF;
            paletteColors[2] = 0x555555FF;
            paletteColors[3] = 0x000000FF;
            break;

        case Palette::Pocket:
            // GB Pocket grey-green
            paletteColors[0] = 0xC4CFA1FF;
            paletteColors[1] = 0x8B956DFF;
            paletteColors[2] = 0x4D533CFF;
            paletteColors[3] = 0x1F1F1FFF;
            break;

        case Palette::Light:
            // GB Light yellowish
            paletteColors[0] = 0xF5F5A8FF;
            paletteColors[1] = 0xC8C87AFF;
            paletteColors[2] = 0x7A7A3CFF;
            paletteColors[3] = 0x1E1E00FF;
            break;

        case Palette::Custom:
            for(int i = 0; i < 4; i++)
                paletteColors[i] = customColors[i];
            break;
    }
}

void Renderer::updateWindowSize()
{
    if(!fullscreen)
    {
        SDL_SetWindowSize(
            window,
            screenWidth  * currentScale,
            screenHeight * currentScale);
    }
}

SDL_FRect Renderer::getDestRect() const
{
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    switch(currentScaleMode)
    {
        case ScaleMode::Stretch:
        {
            return {0, 0, (float)winW, (float)winH};
        }

        case ScaleMode::Integer:
        {
            // largest integer scale that fits
            int scaleX = winW / screenWidth;
            int scaleY = winH / screenHeight;
            int scale  = std::max(1, std::min(scaleX, scaleY));

            float w = screenWidth  * scale;
            float h = screenHeight * scale;
            float x = (winW - w) / 2.0f;
            float y = (winH - h) / 2.0f;
            return {x, y, w, h};
        }

        case ScaleMode::PixelPerfect:
        {
            // fill window preserving aspect ratio
            float scaleX = (float)winW / screenWidth;
            float scaleY = (float)winH / screenHeight;
            float scale  = std::min(scaleX, scaleY);

            float w = screenWidth  * scale;
            float h = screenHeight * scale;
            float x = (winW - w) / 2.0f;
            float y = (winH - h) / 2.0f;
            return {x, y, w, h};
        }
    }

    return {0, 0, (float)winW, (float)winH};
}

void Renderer::drawFrame(const uint8_t* framebuffer)
{
    for(int i = 0; i < screenWidth * screenHeight; i++)
    {
        uint8_t colorIndex = framebuffer[i] & 0x03;
        pixels[i] = paletteColors[colorIndex];
    }

    SDL_UpdateTexture(texture, nullptr, pixels, screenWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);

    SDL_FRect dest = getDestRect();
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
    SDL_RenderPresent(renderer);
}

void Renderer::toggleFullscreen()
{
    fullscreen = !fullscreen;
    SDL_SetWindowFullscreen(window, fullscreen);
}

void Renderer::setScale(int scale)
{
    currentScale = std::max(1, std::min(scale, 8)); // clamp 1-8
    if(!fullscreen)
        updateWindowSize();
}

void Renderer::setPalette(Palette p)
{
    applyPalette(p);
}

void Renderer::setScaleMode(ScaleMode mode)
{
    currentScaleMode = mode;
}

void Renderer::setCustomColors(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3)
{
    customColors[0] = c0;
    customColors[1] = c1;
    customColors[2] = c2;
    customColors[3] = c3;
    if(currentPalette == Palette::Custom)
        applyPalette(Palette::Custom);
}

void Renderer::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}