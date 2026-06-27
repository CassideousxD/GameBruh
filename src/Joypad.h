#pragma once
#include <cstdint>

enum class Button
{
    Right,
    Left,
    Up,
    Down,
    A,
    B,
    Select,
    Start
};

class Joypad
{
private:

    // Row 0 = Direction Keys
    // Bit0 Right
    // Bit1 Left
    // Bit2 Up
    // Bit3 Down

    uint8_t directionKeys;

    // Row 1 = Button Keys
    // Bit0 A
    // Bit1 B
    // Bit2 Select
    // Bit3 Start

    uint8_t buttonKeys;
    uint8_t selectBits;
    bool interruptRequested;

public:

    Joypad();
    void press(Button button);
    void release(Button button);
    void write(uint8_t value);
    uint8_t read() const;
    bool hasInterruptRequest() const;
    void clearInterruptRequest();
    void printState() const;
};