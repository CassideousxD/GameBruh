#include "Joypad.h"
#include <iostream>
#include <bitset>

Joypad::Joypad()
{
    directionKeys = 0x0F;
    buttonKeys = 0x0F;
    selectBits = 0x30;
    interruptRequested = false;
}

void Joypad::press(Button button)
{
    uint8_t* row = nullptr;
    uint8_t bit = 0;
    
    switch(button)
    {
        case Button::Right: row = &directionKeys; bit = 0; break;
        case Button::Left:  row = &directionKeys; bit = 1; break;
        case Button::Up:    row = &directionKeys; bit = 2; break;
        case Button::Down:  row = &directionKeys; bit = 3; break;

        case Button::A:      row = &buttonKeys; bit = 0; break;
        case Button::B:      row = &buttonKeys; bit = 1; break;
        case Button::Select: row = &buttonKeys; bit = 2; break;
        case Button::Start:  row = &buttonKeys; bit = 3; break;
    }

    if (row != nullptr)
    {        bool wasReleased = (*row & (1 << bit)) != 0;
        *row &= ~(1 << bit); // Set to 0 (Pressed)
        if (wasReleased)
        {
            interruptRequested = true;
        }
    }
}

void Joypad::release(Button button)
{
    uint8_t* row;
    uint8_t bit;

    switch(button)
    {
        case Button::Right:
            row = &directionKeys;
            bit = 0;
            break;

        case Button::Left:
            row = &directionKeys;
            bit = 1;
            break;

        case Button::Up:
            row = &directionKeys;
            bit = 2;
            break;

        case Button::Down:
            row = &directionKeys;
            bit = 3;
            break;

        case Button::A:
            row = &buttonKeys;
            bit = 0;
            break;

        case Button::B:
            row = &buttonKeys;
            bit = 1;
            break;

        case Button::Select:
            row = &buttonKeys;
            bit = 2;
            break;

        case Button::Start:
            row = &buttonKeys;
            bit = 3;
            break;
    }
    *row |= (1 << bit);
}

bool Joypad::hasInterruptRequest() const
{
    return interruptRequested;
}

void Joypad::clearInterruptRequest()
{
    interruptRequested = false;
}

void Joypad::write(uint8_t value)
{
    selectBits = value & 0x30;
}

uint8_t Joypad::read() const
{
    uint8_t result = 0xCF;

    result &= ~0x30;
    result |= selectBits;

    if(!(selectBits & 0x10))
    {
        result &= 0xF0;
        result |= directionKeys;
    }

    if(!(selectBits & 0x20))
    {
        result &= 0xF0;
        result |= buttonKeys;
    }
    return result;
}

void Joypad::printState() const
{
    std::cout << "--------------------------------\n";

    std::cout << "Select Bits : "
              << std::bitset<2>(selectBits >> 4)
              << '\n';

    std::cout << "Direction   : "
              << std::bitset<4>(directionKeys)
              << '\n';

    std::cout << "Buttons     : "
              << std::bitset<4>(buttonKeys)
              << '\n';

    std::cout << "Read Value  : "
              << std::bitset<8>(read())
              << "\n\n";
}