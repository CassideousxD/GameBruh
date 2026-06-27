#include "InputManager.h"
#include <iostream>

InputManager::InputManager(Memory& memory)
    : memory(memory)
{
}

void InputManager::update()
{
    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    // Direction Keys (Remapped to WASD)
    if(keyboard[SDL_SCANCODE_D])
        memory.pressButton(Button::Right);
    else
        memory.releaseButton(Button::Right);

    if(keyboard[SDL_SCANCODE_A])
        memory.pressButton(Button::Left);
    else
        memory.releaseButton(Button::Left);

    if(keyboard[SDL_SCANCODE_W])
        memory.pressButton(Button::Up);
    else
        memory.releaseButton(Button::Up);

    if(keyboard[SDL_SCANCODE_S])
        memory.pressButton(Button::Down);
    else
        memory.releaseButton(Button::Down);

    // Action Buttons (Shifted to J/K for ergonomics alongside WASD)
    if(keyboard[SDL_SCANCODE_J])
        memory.pressButton(Button::A);
    else
        memory.releaseButton(Button::A);

    if(keyboard[SDL_SCANCODE_K])
        memory.pressButton(Button::B);
    else
        memory.releaseButton(Button::B);

    if(keyboard[SDL_SCANCODE_SPACE])
        memory.pressButton(Button::Select);
    else
        memory.releaseButton(Button::Select);

    if(keyboard[SDL_SCANCODE_RETURN])
        memory.pressButton(Button::Start);
    else
        memory.releaseButton(Button::Start);
}