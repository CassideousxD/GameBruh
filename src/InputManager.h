#pragma once
#include "Memory.h"
#include <SDL3/SDL.h>

class InputManager
{
private:
    Memory& memory;

    SDL_Gamepad* gamepad = nullptr;

    // deadzone for analog sticks (0-32767)
    static constexpr int DEADZONE = 8000;

    void openGamepad();
    void closeGamepad();

    bool isButtonPressed(Button button) const;
    void updateKeyboard();
    void updateGamepad();

public:
    InputManager(Memory& memory);
    ~InputManager();
    void update();
    void handleEvent(const SDL_Event& event);
};