#include "InputManager.h"
#include <iostream>

InputManager::InputManager(Memory& memory)
    : memory(memory)
{
    // open first available gamepad if connected
    openGamepad();
}

InputManager::~InputManager()
{
    closeGamepad();
}

void InputManager::openGamepad()
{
    int count = 0;
    SDL_JoystickID* joysticks = SDL_GetGamepads(&count);

    if(joysticks && count > 0)
    {
        gamepad = SDL_OpenGamepad(joysticks[0]);
        if(gamepad)
        {
            std::cout << "Controller connected: "
                      << SDL_GetGamepadName(gamepad) << "\n";
        }
    }

    SDL_free(joysticks);
}

void InputManager::closeGamepad()
{
    if(gamepad)
    {
        SDL_CloseGamepad(gamepad);
        gamepad = nullptr;
    }
}

void InputManager::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_EVENT_GAMEPAD_ADDED)
    {
        if(!gamepad)
        {
            gamepad = SDL_OpenGamepad(event.gdevice.which);
            if(gamepad)
            {
                std::cout << "Controller connected: "
                          << SDL_GetGamepadName(gamepad) << "\n";
            }
        }
    }

    if(event.type == SDL_EVENT_GAMEPAD_REMOVED)
    {
        if(gamepad)
        {
            SDL_JoystickID id = SDL_GetGamepadID(gamepad);
            if(id == event.gdevice.which)
            {
                std::cout << "Controller disconnected\n";
                closeGamepad();
            }
        }
    }
}

void InputManager::updateKeyboard()
{
    const bool* kb = SDL_GetKeyboardState(nullptr);

    // D-pad
    if(kb[SDL_SCANCODE_D]) memory.pressButton(Button::Right);
    else                   memory.releaseButton(Button::Right);

    if(kb[SDL_SCANCODE_A]) memory.pressButton(Button::Left);
    else                   memory.releaseButton(Button::Left);

    if(kb[SDL_SCANCODE_W]) memory.pressButton(Button::Up);
    else                   memory.releaseButton(Button::Up);

    if(kb[SDL_SCANCODE_S]) memory.pressButton(Button::Down);
    else                   memory.releaseButton(Button::Down);

    // Buttons
    if(kb[SDL_SCANCODE_J])      memory.pressButton(Button::A);
    else                        memory.releaseButton(Button::A);

    if(kb[SDL_SCANCODE_K])      memory.pressButton(Button::B);
    else                        memory.releaseButton(Button::B);

    if(kb[SDL_SCANCODE_SPACE])  memory.pressButton(Button::Select);
    else                        memory.releaseButton(Button::Select);

    if(kb[SDL_SCANCODE_RETURN]) memory.pressButton(Button::Start);
    else                        memory.releaseButton(Button::Start);
}

void InputManager::updateGamepad()
{
    if(!gamepad) return;

    // --- D-pad buttons ---
    bool dpadRight = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
    bool dpadLeft  = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    bool dpadUp    = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
    bool dpadDown  = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);

    // --- Left analog stick ---
    int16_t axisX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
    int16_t axisY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);

    bool stickRight = axisX >  DEADZONE;
    bool stickLeft  = axisX < -DEADZONE;
    bool stickDown  = axisY >  DEADZONE;
    bool stickUp    = axisY < -DEADZONE;

    // combine dpad and stick
    if(dpadRight || stickRight) memory.pressButton(Button::Right);
    else                        memory.releaseButton(Button::Right);

    if(dpadLeft || stickLeft)   memory.pressButton(Button::Left);
    else                        memory.releaseButton(Button::Left);

    if(dpadUp || stickUp)       memory.pressButton(Button::Up);
    else                        memory.releaseButton(Button::Up);

    if(dpadDown || stickDown)   memory.pressButton(Button::Down);
    else                        memory.releaseButton(Button::Down);

    // --- Face buttons ---
    // A button → controller South (A on Xbox, X on PS)
    bool btnA = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_SOUTH);
    // B button → controller East (B on Xbox, Circle on PS)
    bool btnB = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_EAST);
    // Select → Back/Share/Minus
    bool btnSelect = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_BACK);
    // Start → Start/Options/Plus
    bool btnStart  = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_START);

    if(btnA)      memory.pressButton(Button::A);
    else          memory.releaseButton(Button::A);

    if(btnB)      memory.pressButton(Button::B);
    else          memory.releaseButton(Button::B);

    if(btnSelect) memory.pressButton(Button::Select);
    else          memory.releaseButton(Button::Select);

    if(btnStart)  memory.pressButton(Button::Start);
    else          memory.releaseButton(Button::Start);
}

void InputManager::update()
{
    updateKeyboard();
    updateGamepad();
}