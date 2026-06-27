#pragma once
#include "Memory.h"
#include <SDL3/SDL.h>

class InputManager
{
private:
    Memory& memory;
    bool currentButtonStates[8] = { false };

public:
    InputManager(Memory& memory);
    void update();
    void handleKeyDown(SDL_Keycode key);
    void handleKeyUp(SDL_Keycode key);
};