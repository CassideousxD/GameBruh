#pragma once
#include "Memory.h"
#include <cstdint>

class CPU
{
private:

    uint8_t a;
    uint8_t f;

    uint8_t b;
    uint8_t c;

    uint8_t d;
    uint8_t e;

    uint8_t h;
    uint8_t l;

    uint16_t programCounter;
    uint16_t stackPointer;
    
    Memory& memory;
    uint8_t opcode;

    static constexpr uint8_t flagZ = 0x80;
    static constexpr uint8_t flagN = 0x40;
    static constexpr uint8_t flagH = 0x20;
    static constexpr uint8_t flagC = 0x10;

    void executeNOP();
    void fetchOpcode();
    void loadImmediate(uint8_t& reg);
    void loadRegister(uint8_t& destination, uint8_t source);
    void setFlag(uint8_t flag, bool value);
    bool getFlag(uint8_t flag);
    void incrementRegister(uint8_t& reg);
    void decrementRegister(uint8_t& reg);
    void addRegister(uint8_t reg);
    void subtractRegister(uint8_t reg);
    void andRegister(uint8_t reg);
    void orRegister(uint8_t reg);
    void xorRegister(uint8_t reg);
    void compareRegister(uint8_t reg);
    uint16_t readWord();
    void jump(uint16_t address);
    bool jumpConditional(uint16_t address, bool condition);
    void jumpRelative(int8_t offset);
    bool jumpRelativeConditional(int8_t offset, bool condition);
    void pushStack(uint16_t value);
    uint16_t popStack();
    void callSubroutine(uint16_t address);
    void returnFromSubroutine();
    void requestInterrupt(uint8_t interruptBit);
    uint8_t handleInterrupts(); 
    bool returnConditional(bool condition);
    bool callConditional(uint16_t address, bool condition);
    void loadFromHL(uint8_t& destination);
    void storeToHL(uint8_t source);
    void addHL(uint16_t value);
    void addWithCarry(uint8_t value);
    void subtractWithCarry(uint8_t value);
    void rotateLeftCircularA();
    void rotateRightCircularA();
    void rotateLeftA();
    void rotateRightA();
    void rotateLeftCircular(uint8_t& value);
    void rotateRightCircular(uint8_t& value);
    void rotateLeft(uint8_t& value);
    void rotateRight(uint8_t& value);
    uint8_t executeCBOpcode(uint8_t opcode);
    void shiftLeftArithmetic(uint8_t& value);
    void shiftRightArithmetic(uint8_t& value);
    void shiftRightLogical(uint8_t& value);
    void testBit(uint8_t bit, uint8_t value);
    void resetBit(uint8_t bit, uint8_t& value);
    void setBit(uint8_t bit, uint8_t& value);
    void complementA();
    void setCarryFlag();
    void complementCarryFlag();
    void decimalAdjustAccumulator();

    bool ime = false;
    bool halted = false;
    bool stopped = false;

    static const uint8_t opcodeCycles[256];
    static const uint8_t cbOpcodeCycles[256];

public:

    CPU(Memory& memory);

    uint16_t getAF() const;
    uint16_t getBC() const;
    uint16_t getDE() const;
    uint16_t getHL() const;

    void setAF(uint16_t value);
    void setBC(uint16_t value);
    void setDE(uint16_t value);
    void setHL(uint16_t value);

    uint8_t getA() const;
    uint8_t getF() const;

    uint8_t getB() const;
    uint8_t getC() const;

    uint8_t getD() const;
    uint8_t getE() const;

    uint8_t getH() const;
    uint8_t getL() const;

    uint16_t getProgramCounter() const;
    uint16_t getStackPointer() const;

    uint8_t getOpcode() const;
    uint8_t executeOpcode();

    bool isHalted() const;
    bool isStopped() const;
    bool getIME() const;

    void printState() const;
    
};