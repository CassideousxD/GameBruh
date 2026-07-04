#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <string>

enum class ScaleMode
{
    Integer,      // nearest neighbor, integer scale only
    PixelPerfect, // same as integer but locked to aspect ratio
    Stretch       // fill window ignoring aspect ratio
};

enum class Palette
{
    DMG,          // original green
    Grayscale,    // white to black
    Pocket,       // grey-green like GB Pocket
    Light,        // GB Light yellowish
    Custom        // user defined
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void drawFrame(const uint8_t* framebuffer);

    // Window
    void toggleFullscreen();
    void setScale(int scale);
    int  getScale() const { return currentScale; }
    bool isFullscreen() const { return fullscreen; }

    // Palette
    void setPalette(Palette p);
    Palette getPalette() const { return currentPalette; }

    // Scale mode
    void setScaleMode(ScaleMode mode);
    ScaleMode getScaleMode() const { return currentScaleMode; }

    // Custom palette
    void setCustomColors(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3);

    // Window title
    void setTitle(const std::string& title);

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    static constexpr int screenWidth  = 160;
    static constexpr int screenHeight = 144;

    int       currentScale    = 4;
    bool      fullscreen      = false;
    Palette   currentPalette  = Palette::Grayscale;
    ScaleMode currentScaleMode = ScaleMode::Integer;

    uint32_t pixels[160 * 144] = {};

    // Palette color arrays [color0..color3] in RGBA8888
    uint32_t paletteColors[4] = {};
    uint32_t customColors[4]  = {
        0xFFFFFFFF,
        0xAAAAAAFF,
        0x555555FF,
        0x000000FF
    };

    void applyPalette(Palette p);
    void updateWindowSize();
    SDL_FRect getDestRect() const;
};