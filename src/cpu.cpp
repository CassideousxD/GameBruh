#include "CPU.h"
#include<iostream>
#include <iomanip>

CPU::CPU(Memory &memory):memory(memory)
{
    a = 0x00;
    f = 0x00;
    b = 0x00;
    c = 0x00;
    d = 0x00;
    e = 0x00;
    h = 0x00;
    l = 0x00;
    programCounter = 0x0000;
    stackPointer = 0x0000;  
    opcode = 0x00;
    halted = false;
    stopped = false;
    ime = false;
}

const uint8_t CPU::opcodeCycles[256] =
{
    // 0x00 - 0x0F
     4,12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4,
    // 0x10 - 0x1F
     4,12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
    // 0x20 - 0x2F
     8,12, 8, 8, 4, 4, 8, 4,  8, 8, 8, 8, 4, 4, 8, 4,
    // 0x30 - 0x3F
     8,12, 8, 8,12,12,12, 4,  8, 8, 8, 8, 4, 4, 8, 4,
    // 0x40 - 0x4F
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0x50 - 0x5F
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0x60 - 0x6F
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0x70 - 0x7F
     8, 8, 8, 8, 8, 8, 4, 8,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0x80 - 0x8F
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0x90 - 0x9F
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0xA0 - 0xAF
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0xB0 - 0xBF
     4, 4, 4, 4, 4, 4, 8, 4,  4, 4, 4, 4, 4, 4, 8, 4,
    // 0xC0 - 0xCF
     8,12,12,16,12,16, 8,16,  8,16,12, 4,12,24, 8,16,
    // 0xD0 - 0xDF
     8,12,12, 0,12,16, 8,16,  8,16,12, 0,12, 0, 8,16,
    // 0xE0 - 0xEF
    12,12, 8, 0, 0,16, 8,16, 16, 4,16, 0, 0, 0, 8,16,
    // 0xF0 - 0xFF
    12,12, 8, 4, 0,16, 8,16, 12, 8,16, 4, 0, 0, 8,16
};

const uint8_t CPU::cbOpcodeCycles[256] =
{
    // 00-3F
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
    // 40-7F
     8,8,8,8,8,8,12,8, 8,8,8,8,8,8,12,8,
     8,8,8,8,8,8,12,8, 8,8,8,8,8,8,12,8,
     8,8,8,8,8,8,12,8, 8,8,8,8,8,8,12,8,
     8,8,8,8,8,8,12,8, 8,8,8,8,8,8,12,8,
    // 80-BF
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
    // C0-FF
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8,
     8,8,8,8,8,8,16,8, 8,8,8,8,8,8,16,8
};

uint16_t CPU::getAF() const
{
    return (static_cast<uint16_t>(a) << 8) | f;
}

uint16_t CPU::getBC() const
{
    return (static_cast<uint16_t>(b) << 8) | c;
}

uint16_t CPU::getDE() const
{
    return (static_cast<uint16_t>(d) << 8) | e;
}

uint16_t CPU::getHL() const
{
    return (static_cast<uint16_t>(h) << 8) | l;
}

void CPU::setAF(uint16_t value)
{
    a = static_cast<uint8_t>(value >> 8);
    f = static_cast<uint8_t>(value & 0xF0); 
}

void CPU::setBC(uint16_t value)
{
    b = static_cast<uint8_t>(value >> 8);
    c = static_cast<uint8_t>(value & 0xFF);
}

void CPU::setDE(uint16_t value)
{
    d = static_cast<uint8_t>(value >> 8);
    e = static_cast<uint8_t>(value & 0xFF);
}

void CPU::setHL(uint16_t value)
{
    h = static_cast<uint8_t>(value >> 8);
    l = static_cast<uint8_t>(value & 0xFF);
}

uint8_t CPU::getD() const
{
    return d;
}

uint8_t CPU::getE() const
{
    return e;
}

uint8_t CPU::getB() const
{
    return b;
}

uint8_t CPU::getC() const
{
    return c;
}

uint8_t CPU::getA() const
{
    return a;
}

uint8_t CPU::getF() const
{
    return f;
}

uint8_t CPU::getH() const
{
    return h;
}

uint8_t CPU::getL() const
{
    return l;
}

uint16_t CPU::getStackPointer() const
{
    return stackPointer;
}

uint16_t CPU::getProgramCounter() const
{
    return programCounter;
}

void CPU::fetchOpcode()
{
    opcode = memory.readByte(programCounter);
}

uint8_t CPU::getOpcode() const
{
    return opcode;
}

void CPU::executeNOP()
{
    programCounter += 1;
}

void CPU::loadImmediate(uint8_t& reg)
{
    reg = memory.readByte(programCounter + 1);
    programCounter += 2;
}

void CPU::loadRegister(uint8_t& destination, uint8_t source)
{
    destination = source;
    programCounter += 1;
}

void CPU::setFlag(uint8_t flag, bool value)
{
    if (value)
    {
        f |= flag;
    }
    else
    {
        f &= ~flag;
    }
    f &= 0xF0;
}

bool CPU::getFlag(uint8_t flag)
{
    return (f & flag) != 0;
}

void CPU::incrementRegister(uint8_t& reg)
{
    bool halfCarry = (reg & 0x0F) == 0x0F;
    reg++;
    setFlag(flagZ, reg == 0);
    setFlag(flagN, false);
    setFlag(flagH, halfCarry);
    programCounter++;
}

void CPU::decrementRegister(uint8_t& reg)
{
    bool halfBorrow = (reg & 0x0F) == 0x00;
    reg--;
    setFlag(flagZ, reg == 0);
    setFlag(flagN, true);
    setFlag(flagH, halfBorrow);
    programCounter++;
}

void CPU::addRegister(uint8_t reg)
{
    uint16_t result = a + reg;
    setFlag(flagZ, (result & 0xFF) == 0);
    setFlag(flagN, false);
    setFlag(flagH, ((a & 0x0F) + (reg & 0x0F)) > 0x0F);
    setFlag(flagC, result > 0xFF);
    a = static_cast<uint8_t>(result);
    programCounter++;
}

void CPU::subtractRegister(uint8_t reg)
{
    int result = static_cast<int>(a) - static_cast<int>(reg);
    
    setFlag(flagZ, (result & 0xFF) == 0);
    setFlag(flagN, true);
    setFlag(flagH, (static_cast<int>(a & 0x0F) - static_cast<int>(reg & 0x0F)) < 0);
    setFlag(flagC, result < 0);
    
    a = static_cast<uint8_t>(result);
    programCounter++;
}

void CPU::andRegister(uint8_t reg)
{
    a &= reg;
    setFlag(flagZ, a == 0);
    setFlag(flagN, false);
    setFlag(flagH, true);
    setFlag(flagC, false);
    programCounter++;
}

void CPU::orRegister(uint8_t reg)
{
    a |= reg;
    setFlag(flagZ, a == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, false);
    programCounter++;
}

void CPU::xorRegister(uint8_t reg)
{
    a ^= reg;
    setFlag(flagZ, a == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, false);
    programCounter++;
}

void CPU::compareRegister(uint8_t reg)
{
    int result = static_cast<int>(a) - static_cast<int>(reg);
    
    setFlag(flagZ, (result & 0xFF) == 0);
    setFlag(flagN, true);
    setFlag(flagH, (static_cast<int>(a & 0x0F) - static_cast<int>(reg & 0x0F)) < 0);
    
    // Carry tracking
    setFlag(flagC, result < 0);
    programCounter++;
}

uint16_t CPU::readWord()
{
    uint8_t lowByte = memory.readByte(programCounter + 1);
    uint8_t highByte = memory.readByte(programCounter + 2);
    return lowByte | (highByte << 8);
}

void CPU::jump(uint16_t address)
{
    programCounter = address;
}

bool CPU::jumpConditional(uint16_t address, bool condition)
{
    if(condition)
    {
        programCounter = address;
        return true;
    }

    programCounter += 3;
    return false;
}

void CPU::jumpRelative(int8_t offset)
{
    programCounter = programCounter + 2 + offset;
}

bool CPU::jumpRelativeConditional(int8_t offset, bool condition)
{
    if(condition)
    {
        jumpRelative(offset);
        return true;
    }

    programCounter += 2;
    return false;
}

void CPU::pushStack(uint16_t value)
{
    uint8_t lowByte = value & 0xFF;
    uint8_t highByte = value >> 8;
    stackPointer--;
    memory.writeByte(stackPointer, highByte);
    stackPointer--;
    memory.writeByte(stackPointer, lowByte);
}

uint16_t CPU::popStack()
{
    uint8_t lowByte = memory.readByte(stackPointer);
    stackPointer++;
    uint8_t highByte = memory.readByte(stackPointer);
    stackPointer++;
    return static_cast<uint16_t>(lowByte) | (static_cast<uint16_t>(highByte) << 8);
}

void CPU::callSubroutine(uint16_t address)
{
    pushStack(programCounter + 3);
    jump(address);
}

void CPU::returnFromSubroutine()
{
    programCounter = popStack();
}

void CPU::requestInterrupt(uint8_t interruptBit)
{
    uint8_t interruptFlag = memory.readByte(0xFF0F);
    interruptFlag |= (1 << interruptBit);
    memory.writeByte(0xFF0F, interruptFlag);
}

uint8_t CPU::handleInterrupts()
{
    uint8_t interruptEnable = memory.readByte(0xFFFF);
    uint8_t interruptFlag = memory.readByte(0xFF0F);
    uint8_t pendingInterrupts = interruptEnable & interruptFlag;
    
    if(pendingInterrupts == 0) return 0; 
    if(halted) halted = false;
    
    if(!ime) return 0; // IME only blocks jumping to the vector table
    
    ime = false;
    if(pendingInterrupts & 0x01) // V-Blank
    {
        interruptFlag &= ~0x01;
        memory.writeByte(0xFF0F, interruptFlag); // Clear in memory bank
        pushStack(programCounter);
        jump(0x0040);
        return 20; 
    }

    if(pendingInterrupts & 0x02)
    {
        interruptFlag &= ~0x02;
        memory.writeByte(0xFF0F, interruptFlag);
        pushStack(programCounter);
        jump(0x0048);
        return 20;
    }

    if(pendingInterrupts & 0x04)
    {
        interruptFlag &= ~0x04;
        memory.writeByte(0xFF0F, interruptFlag);
        pushStack(programCounter);
        jump(0x0050);
        return 20;
    }

    if(pendingInterrupts & 0x08)
    {
        interruptFlag &= ~0x08;
        memory.writeByte(0xFF0F, interruptFlag);
        pushStack(programCounter);
        jump(0x0058);
        return 20;
    }

    if(pendingInterrupts & 0x10)
    {
        interruptFlag &= ~0x10; 
        memory.writeByte(0xFF0F, interruptFlag); 
        pushStack(programCounter);
        jump(0x0060);
        return 20;
    }

    return 0;
}

bool CPU::returnConditional(bool condition)
{
    if(condition)
    {
        returnFromSubroutine();
        return true;
    }

    programCounter += 1;
    return false;
}

bool CPU::callConditional(uint16_t address, bool condition)
{
    if(condition)
    {
        callSubroutine(address);
        return true;
    }

    programCounter += 3;
    return false;
}

void CPU::loadFromHL(uint8_t& destination)
{
    destination = memory.readByte(getHL());
    programCounter++;
}

void CPU::storeToHL(uint8_t source)
{
    memory.writeByte(getHL(), source);
    programCounter++;
}

void CPU::addHL(uint16_t value)
{
    uint32_t result = getHL() + value;
    setFlag(flagN, false);
    setFlag(flagH,((getHL() & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
    setFlag(flagC,result > 0xFFFF);
    setHL(static_cast<uint16_t>(result));
}

void CPU::addWithCarry(uint8_t value)
{
    int carry = getFlag(flagC) ? 1 : 0;
    int result = static_cast<int>(a) + static_cast<int>(value) + carry;
    
    setFlag(flagZ, (result & 0xFF) == 0);
    setFlag(flagN, false);
    setFlag(flagH, ((a & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    setFlag(flagC, result > 0xFF);
    
    a = static_cast<uint8_t>(result);
    programCounter++;
}

void CPU::subtractWithCarry(uint8_t value)
{
    int carry = getFlag(flagC) ? 1 : 0;
    int result = static_cast<int>(a) - static_cast<int>(value) - carry; 
    setFlag(flagZ, (result & 0xFF) == 0);
    setFlag(flagN, true);
    setFlag(flagH, (static_cast<int>(a & 0x0F) - static_cast<int>(value & 0x0F) - carry) < 0);
    setFlag(flagC, result < 0);  
    a = static_cast<uint8_t>(result);
    programCounter++;
}

void CPU::rotateLeftCircularA()
{
    uint8_t bit7 = (a >> 7) & 0x01;
    a = (a << 1) | bit7;
    setFlag(flagZ, false);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit7);
}

void CPU::rotateRightCircularA()
{
    uint8_t bit0 = a & 0x01;
    a = (a >> 1) | (bit0 << 7);
    setFlag(flagZ, false);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::rotateLeftA()
{
    uint8_t oldCarry = getFlag(flagC) ? 1 : 0;
    uint8_t bit7 = (a >> 7) & 0x01;
    a = (a << 1) | oldCarry;
    setFlag(flagZ, false);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit7);
}

void CPU::rotateRightA()
{
    uint8_t oldCarry = getFlag(flagC) ? 1 : 0;
    uint8_t bit0 = a & 0x01;
    a = (a >> 1) | (oldCarry << 7);
    setFlag(flagZ, false);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::rotateLeftCircular(uint8_t& value)
{
    uint8_t bit7 = (value >> 7) & 0x01;
    value = (value << 1) | bit7;
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit7);
}

void CPU::rotateRightCircular(uint8_t& value)
{
    uint8_t bit0 = value & 0x01;
    value = (value >> 1) | (bit0 << 7);
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::rotateLeft(uint8_t& value)
{
    uint8_t oldCarry = getFlag(flagC) ? 1 : 0;
    uint8_t bit7 = (value >> 7) & 0x01;
    value = (value << 1) | oldCarry;
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit7);
}

void CPU::rotateRight(uint8_t& value)
{
    uint8_t oldCarry = getFlag(flagC) ? 1 : 0;
    uint8_t bit0 = value & 0x01;
    value = (value >> 1) | (oldCarry << 7);
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::shiftLeftArithmetic(uint8_t& value)
{
    uint8_t bit7 = (value >> 7) & 0x01;
    value <<= 1;
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit7);
}

void CPU::shiftRightArithmetic(uint8_t& value)
{
    uint8_t bit0 = value & 0x01;
    uint8_t bit7 = value & 0x80;
    value = (value >> 1) | bit7;
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::shiftRightLogical(uint8_t& value)
{
    uint8_t bit0 = value & 0x01;
    value >>= 1;
    setFlag(flagZ, value == 0);
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, bit0);
}

void CPU::testBit(uint8_t bit, uint8_t value)
{
    bool isSet = value & (1 << bit);

    setFlag(flagZ, !isSet);
    setFlag(flagN, false);
    setFlag(flagH, true);
}

void CPU::resetBit(uint8_t bit, uint8_t& value)
{
    value &= ~(1 << bit);
}

void CPU::setBit(uint8_t bit, uint8_t& value)
{
    value |= (1 << bit);
}

void CPU::complementA()
{
    a = ~a;

    setFlag(flagN, true);
    setFlag(flagH, true);
}

void CPU::setCarryFlag()
{
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC, true);
}

void CPU::complementCarryFlag()
{
    setFlag(flagN, false);
    setFlag(flagH, false);
    setFlag(flagC,!getFlag(flagC));
}

void CPU::printState() const
{
    std::cout
        << std::uppercase
        << std::hex
        << std::setfill('0');

    std::cout
        << "PC:"
        << std::setw(4)
        << programCounter
        << "  ";

    std::cout
        << "OP:"
        << std::setw(2)
        << (int)opcode
        << "  ";

    std::cout
        << "AF:"
        << std::setw(2)
        << (int)a
        << std::setw(2)
        << (int)f
        << "  ";

    std::cout
        << "BC:"
        << std::setw(2)
        << (int)b
        << std::setw(2)
        << (int)c
        << "  ";

    std::cout
        << "DE:"
        << std::setw(2)
        << (int)d
        << std::setw(2)
        << (int)e
        << "  ";

    std::cout
        << "HL:"
        << std::setw(2)
        << (int)h
        << std::setw(2)
        << (int)l
        << "  ";

    std::cout
        << "SP:"
        << std::setw(4)
        << stackPointer
        << '\n';
}

void CPU::decimalAdjustAccumulator()
{
    uint8_t correction = 0;
    bool setCarry = false;
    if (!getFlag(flagN))
    {
        if (getFlag(flagH) || (a & 0x0F) > 0x09)
        {
            correction |= 0x06;
        }
        if (getFlag(flagC) || a > 0x99)
        {
            correction |= 0x60;
            setCarry = true;
        }
        a += correction;
    }
    else
    {
        if (getFlag(flagH))
        {
            correction |= 0x06;
        }
        if (getFlag(flagC))
        {
            correction |= 0x60;
            setCarry = true;
        }
        a -= correction;
    }
    setFlag(flagZ, a == 0);
    setFlag(flagH, false);
    setFlag(flagC, setCarry || getFlag(flagC)); 
}

uint8_t CPU::executeOpcode()
{
    uint8_t interruptCycles = handleInterrupts();
    if(halted)
    {
        return 4 + interruptCycles;
    }
    fetchOpcode();
    uint8_t cycles = opcodeCycles[opcode] + interruptCycles;

    switch(opcode)
    {
        //No operation
        case 0x00:
            executeNOP();
            break;
        //Increment registers
        case 0x04:
            incrementRegister(b);
            break;

        case 0x0C:
            incrementRegister(c);
            break;

        case 0x14:
            incrementRegister(d);
            break;

        case 0x1C:
            incrementRegister(e);
            break;

        case 0x24:
            incrementRegister(h);
            break;

        case 0x2C:
            incrementRegister(l);
            break;

        case 0x3C:
            incrementRegister(a);
            break;

        //decrement registers
        case 0x05:
            decrementRegister(b);
            break;

        case 0x0D:
            decrementRegister(c);
            break;

        case 0x15:
            decrementRegister(d);
            break;

        case 0x1D:
            decrementRegister(e);
            break;

        case 0x25:
            decrementRegister(h);
            break;

        case 0x2D:
            decrementRegister(l);
            break;

        case 0x3D:
            decrementRegister(a);
            break;

        //Load value to register
        case 0x3E:
            loadImmediate(a);
            break;

        case 0x06:
            loadImmediate(b);
            break;

        case 0x0E:
            loadImmediate(c);
            break;

        case 0x16:
            loadImmediate(d);
            break;

        case 0x1E:
            loadImmediate(e);
            break;

        case 0x26:
            loadImmediate(h);
            break;

        case 0x2E:
            loadImmediate(l);
            break;

        //load register to A
        case 0x78:
            loadRegister(a, b);
            break;

        case 0x79:
            loadRegister(a, c);
            break;

        case 0x7A:
            loadRegister(a, d);
            break;

        case 0x7B:
            loadRegister(a, e);
            break;

        case 0x7C:
            loadRegister(a, h);
            break;

        case 0x7D:
            loadRegister(a, l);
            break;

        case 0x7F:
            loadRegister(a, a);
            break;

        //add two registers
        case 0x80:
            addRegister(b);
            break;

        case 0x81:
            addRegister(c);
            break;

        case 0x82:
            addRegister(d);
            break;

        case 0x83:
            addRegister(e);
            break;

        case 0x84:
            addRegister(h);
            break;

        case 0x85:
            addRegister(l);
            break;

        case 0x87:
            addRegister(a);
            break;

        //add immediate value 
        case 0xC6:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            addRegister(value);
            break;
        }

        //subtract register
        case 0x90:
            subtractRegister(b);
            break;

        case 0x91:
            subtractRegister(c);
            break;

        case 0x92:
            subtractRegister(d);
            break;

        case 0x93:
            subtractRegister(e);
            break;

        case 0x94:
            subtractRegister(h);
            break;

        case 0x95:
            subtractRegister(l);
            break;

        case 0x97:
            subtractRegister(a);
            break;

        //subtract immediate
        case 0xD6:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            subtractRegister(value);
            break;
        }

        //bitwise AND
        case 0xA0:
            andRegister(b);
            break;

        case 0xA1:
            andRegister(c);
            break;

        case 0xA2:
            andRegister(d);
            break;

        case 0xA3:
            andRegister(e);
            break;

        case 0xA4:
            andRegister(h);
            break;

        case 0xA5:
            andRegister(l);
            break;

        case 0xA7:
            andRegister(a);
            break;

        //immediate biwise AND
        case 0xE6:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            andRegister(value);
            break;
        }

        //bitwise OR
        case 0xB0:
            orRegister(b);
            break;

        case 0xB1:
            orRegister(c);
            break;

        case 0xB2:
            orRegister(d);
            break;

        case 0xB3:
            orRegister(e);
            break;

        case 0xB4:
            orRegister(h);
            break;

        case 0xB5:
            orRegister(l);
            break;

        case 0xB7:
            orRegister(a);
            break;

        //immediate bitwise OR
        case 0xF6:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            orRegister(value);
            break;
        }

        //bitwise XOR
        case 0xA8:
            xorRegister(b);
            break;

        case 0xA9:
            xorRegister(c);
            break;

        case 0xAA:
            xorRegister(d);
            break;

        case 0xAB:
            xorRegister(e);
            break;

        case 0xAC:
            xorRegister(h);
            break;

        case 0xAD:
            xorRegister(l);
            break;

        case 0xAF:
            xorRegister(a);
            break;

        //immediate bitwise XOR
        case 0xEE:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            xorRegister(value);
            break;
        }

        //compare registers
        case 0xB8:
            compareRegister(b);
            break;

        case 0xB9:
            compareRegister(c);
            break;

        case 0xBA:
            compareRegister(d);
            break;

        case 0xBB:
            compareRegister(e);
            break;

        case 0xBC:
            compareRegister(h);
            break;

        case 0xBD:
            compareRegister(l);
            break;

        case 0xBF:
            compareRegister(a);
            break;

        //compare immediate
        case 0xFE:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            compareRegister(value);
            break;
        }

        //jump
        case 0xC3:
            jump(readWord());
            break;

        //jump if flagz not zero
        case 0xC2:
        {
            uint16_t address = readWord();
            bool taken = jumpConditional(address, !getFlag(flagZ));
            cycles = taken ? 16 : 12;
            break;
        }

        //jump if flagz=zero
        case 0xCA:
        {
            uint16_t address = readWord();
            bool taken = jumpConditional(address, getFlag(flagZ));
            cycles = taken ? 16 : 12;
            break;
        }

        //jump if flagc not zero
        case 0xD2:
        {
            uint16_t address = readWord();
            bool taken = jumpConditional(address, !getFlag(flagC));
            cycles = taken ? 16 : 12;
            break;
        }

        //jump if flagc is zero
        case 0xDA:
        {
            uint16_t address = readWord();
            bool taken = jumpConditional(address, getFlag(flagC));
            cycles = taken ? 16 : 12;
            break;
        }

        //jump relative
        case 0x18:
        {
            int8_t offset =static_cast<int8_t>(memory.readByte(programCounter + 1));
            jumpRelative(offset);
            break;
        }

        //jump relative if flagz is not zero
        case 0x20:
        {
            int8_t offset = static_cast<int8_t>(memory.readByte(programCounter + 1));
            bool taken = jumpRelativeConditional(offset, !getFlag(flagZ));
            cycles = taken ? 12 : 8;
            break;
        }

        //jump relative if flagz is zero
        case 0x28:
        {
            int8_t offset = static_cast<int8_t>(memory.readByte(programCounter + 1));
            bool taken = jumpRelativeConditional(offset, getFlag(flagZ));
            cycles = taken ? 12 : 8;
            break;
        }

        //jump relative if flagc is not zero
        case 0x30:
        {
            int8_t offset = static_cast<int8_t>(memory.readByte(programCounter + 1));
            bool taken = jumpRelativeConditional(offset, !getFlag(flagC));
            cycles = taken ? 12 : 8;
            break;
        }

        //jump relative if flac is zero
        case 0x38:
        {
            int8_t offset = static_cast<int8_t>(memory.readByte(programCounter + 1));
            bool taken = jumpRelativeConditional(offset, getFlag(flagC));
            cycles = taken ? 12 : 8;
            break;
        }

        //call subroutine
        case 0xCD:
        {
            callSubroutine(readWord());
            break;
        }

        //return from subroutine
        case 0xC9:
        {
            returnFromSubroutine();
            return 16;
            break;
        }

        //return if flagz not zero
        case 0xC0:
        {
            bool taken = returnConditional(!getFlag(flagZ));
            cycles = taken ? 20 : 8;
            break;
        }

        //return if flag zero
        case 0xC8:
        {
            bool taken = returnConditional(getFlag(flagZ));
            cycles = taken ? 20 : 8;
            break;
        }

        //return if flagc not zero
        case 0xD0:
        {
            bool taken = returnConditional(!getFlag(flagC));
            cycles = taken ? 20 : 8;
            break;
        }

        //return if flagc zero
        case 0xD8:
        {
            bool taken = returnConditional(getFlag(flagC));
            cycles = taken ? 20 : 8;
            break;
        }

        //call if flagz not zero
        case 0xC4:
        {
            uint16_t address = readWord();
            bool taken = callConditional(address, !getFlag(flagZ));
            cycles = taken ? 24 : 12;
            break;
        }

        //call if flagz is zero
        case 0xCC:
        {
            uint16_t address = readWord();
            bool taken = callConditional(address, getFlag(flagZ));
            cycles = taken ? 24 : 12;
            break;
        }

        //call if flagc is not zero
        case 0xD4:
        {
            uint16_t address = readWord();
            bool taken = callConditional(address, !getFlag(flagC));
            cycles = taken ? 24 : 12;
            break;
        }

        //call if flagc is zero
        case 0xDC:
        {
            uint16_t address = readWord();
            bool taken = callConditional(address, getFlag(flagC));
            cycles = taken ? 24 : 12;
            break;
        }

        //load immediate 16bit to RR
        case 0x01:
            setBC(readWord());
            programCounter += 3;
            break;

        case 0x11:
            setDE(readWord());
            programCounter += 3;
            break;

        case 0x21:
            setHL(readWord());
            programCounter += 3;
            break;

        //increment 16bit registers
        case 0x03:
            setBC(getBC() + 1);
            programCounter += 1;
            break;

        case 0x13:
            setDE(getDE() + 1);
            programCounter += 1;
            break;

        case 0x23:
            setHL(getHL() + 1);
            programCounter += 1;
            break;

        //decrement 16bit registers
        case 0x0B:
            setBC(getBC() - 1);
            programCounter += 1;
            break;

        case 0x1B:
            setDE(getDE() - 1);
            programCounter += 1;
            break;

        case 0x2B:
            setHL(getHL() - 1);
            programCounter += 1;
            break;

        //add to HL
        case 0x09:
            addHL(getBC());
            programCounter++;
            break;

        case 0x19:
            addHL(getDE());
            programCounter++;
            break;

        case 0x29:
            addHL(getHL());
            programCounter++;
            break;

        case 0x39:
            addHL(stackPointer);
            programCounter++;
            break;

        //load immediate to SP
        case 0x31:
            stackPointer = readWord();
            programCounter += 3;
            break;

        //increment SP
        case 0x33:
            stackPointer++;
            programCounter += 1;
            break;

        //decrement SP
        case 0x3B:
            stackPointer--;
            programCounter += 1;
            break;

        //load HL to A
        case 0x7E:
            a = memory.readByte(getHL());
            programCounter += 1;
            break;

        //load A to HL
        case 0x77:
            memory.writeByte(getHL(), a);
            programCounter += 1;
            break;

        //load immediate to HL
        case 0x36:
            memory.writeByte(getHL(), memory.readByte(programCounter + 1));
            programCounter += 2;
            break;

        //increment HL
        case 0x34:
        {
            uint8_t value = memory.readByte(getHL());
            incrementRegister(value);
            memory.writeByte(getHL(), value);
            break;
        }

        //decrement HL
        case 0x35:
        {
            uint8_t value = memory.readByte(getHL());
            decrementRegister(value);
            memory.writeByte(getHL(), value);
            break;
        }

        //load from HL to registers
        case 0x46:
            loadFromHL(b);
            break;

        case 0x4E:
            loadFromHL(c);
            break;

        case 0x56:
            loadFromHL(d);
            break;

        case 0x5E:
            loadFromHL(e);
            break;

        case 0x66:
            loadFromHL(h);
            break;

        case 0x6E:
            loadFromHL(l);
            break;

        //load from registers to HL
        case 0x70:
            storeToHL(b);
            break;

        case 0x71:
            storeToHL(c);
            break;

        case 0x72:
            storeToHL(d);
            break;

        case 0x73:
            storeToHL(e);
            break;

        case 0x74:
            storeToHL(h);
            break;

        case 0x75:
            storeToHL(l);
            break;

        //push register pairs into stack
        case 0xC5:
            pushStack(getBC());
            programCounter++;
            break;

        case 0xD5:
            pushStack(getDE());
            programCounter++;
            break;

        case 0xE5:
            pushStack(getHL());
            programCounter++;
            break;

        case 0xF5:
            pushStack((a << 8) | f);
            programCounter++;
            break;

        //pop register pair from stack
        case 0xC1:
            setBC(popStack());
            programCounter++;
            break;

        case 0xD1:
            setDE(popStack());
            programCounter++;
            break;

        case 0xE1:
            setHL(popStack());
            programCounter++;
            break;

        case 0xF1:
        {
            uint16_t af = popStack();
            a = af >> 8;
            f = af & 0xF0;
            programCounter++;
            break;
        }

        //load registers to B
        case 0x40:
            loadRegister(b, b);
            break;

        case 0x41:
            loadRegister(b, c);
            break;

        case 0x42:
            loadRegister(b, d);
            break;

        case 0x43:
            loadRegister(b, e);
            break;

        case 0x44:
            loadRegister(b, h);
            break;

        case 0x45:
            loadRegister(b, l);
            break;

        case 0x47:
            loadRegister(b, a);
            break;

        //load registers to C
        case 0x48:
            loadRegister(c, b);
            break;

        case 0x49:
            loadRegister(c, c);
            break;

        case 0x4A:
            loadRegister(c, d);
            break;

        case 0x4B:
            loadRegister(c, e);
            break;

        case 0x4C:
            loadRegister(c, h);
            break;

        case 0x4D:
            loadRegister(c, l);
            break;

        case 0x4F:
            loadRegister(c, a);
            break;

        //load registers to D
        case 0x50:
            loadRegister(d, b);
            break;

        case 0x51:
            loadRegister(d, c);
            break;

        case 0x52:
            loadRegister(d, d);
            break;

        case 0x53:
            loadRegister(d, e);
            break;

        case 0x54:
            loadRegister(d, h);
            break;

        case 0x55:
            loadRegister(d, l);
            break;

        case 0x57:
            loadRegister(d, a);
            break;

        //load registers to E
        case 0x58:
            loadRegister(e, b);
            break;

        case 0x59:
            loadRegister(e, c);
            break;

        case 0x5A:
            loadRegister(e, d);
            break;

        case 0x5B:
            loadRegister(e, e);
            break;

        case 0x5C:
            loadRegister(e, h);
            break;

        case 0x5D:
            loadRegister(e, l);
            break;

        case 0x5F:
            loadRegister(e, a);
            break;

        //load registers to H
        case 0x60:
            loadRegister(h, b);
            break;

        case 0x61:
            loadRegister(h, c);
            break;

        case 0x62:
            loadRegister(h, d);
            break;

        case 0x63:
            loadRegister(h, e);
            break;

        case 0x64:
            loadRegister(h, h);
            break;

        case 0x65:
            loadRegister(h, l);
            break;

        case 0x67:
            loadRegister(h, a);
            break;

        //load registers to L
        case 0x68:
            loadRegister(l, b);
            break;

        case 0x69:
            loadRegister(l, c);
            break;

        case 0x6A:
            loadRegister(l, d);
            break;

        case 0x6B:
            loadRegister(l, e);
            break;

        case 0x6C:
            loadRegister(l, h);
            break;

        case 0x6D:
            loadRegister(l, l);
            break;

        case 0x6F:
            loadRegister(l, a);
            break;

        //add with carry
        case 0x88:
            addWithCarry(b);
            break;

        case 0x89:
            addWithCarry(c);
            break;

        case 0x8A:
            addWithCarry(d);
            break;

        case 0x8B:
            addWithCarry(e);
            break;

        case 0x8C:
            addWithCarry(h);
            break;

        case 0x8D:
            addWithCarry(l);
            break;

        case 0x8F:
            addWithCarry(a);
            break;

        //immediate add with carry
        case 0xCE:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;  // skip immediate byte
            addWithCarry(value);  // this does the final PC++
            break;
        }

        //subtract with carry
        case 0x98:
            subtractWithCarry(b);
            break;

        case 0x99:
            subtractWithCarry(c);
            break;

        case 0x9A:
            subtractWithCarry(d);
            break;

        case 0x9B:
            subtractWithCarry(e);
            break;

        case 0x9C:
            subtractWithCarry(h);
            break;

        case 0x9D:
            subtractWithCarry(l);
            break;

        case 0x9F:
            subtractWithCarry(a);
            break;

        //immediate subtract with carry
        case 0xDE:
        {
            uint8_t value = memory.readByte(programCounter + 1);
            programCounter++;
            subtractWithCarry(value);
            break;
        }

        //rotate left circular A
        case 0x07:
            rotateLeftCircularA();
            programCounter++;
            break;

        //rotate right circular A
        case 0x0F:
            rotateRightCircularA();
            programCounter++;
            break;

        //rotate left A
        case 0x17:
            rotateLeftA();
            programCounter++;
            break;

        //rotate right A
        case 0x1F:
            rotateRightA();
            programCounter++;
            break;

        //CB opcodes
        case 0xCB:
        {
            uint8_t cbOpcode = memory.readByte(programCounter + 1);
            cycles = executeCBOpcode(cbOpcode) ; 
            programCounter += 2;
            return cycles;
        }

        //Complement A
        case 0x2F:
            complementA();
            programCounter++;
            break;

        //set carry flag
        case 0x37:
            setCarryFlag();
            programCounter++;
            break;

        //complement carry flag
        case 0x3F:
            complementCarryFlag();
            programCounter++;
            break;

        //decimal adjust accumulator
        case 0x27:
            decimalAdjustAccumulator();
            programCounter++;
            break;

        case 0x32:
        {
            memory.writeByte(getHL(), a);
            setHL(getHL() - 1);
            programCounter += 1;
            break;
        }

        case 0x02:
        {
            memory.writeByte(getBC(), a);
            programCounter += 1;
            break;
        }

        case 0x0A:
        {
            a = memory.readByte(getBC());
            programCounter += 1;
            break;
        }

        case 0x12:
        {
            memory.writeByte(getDE(), a);
            programCounter += 1;
            break;
        }

        case 0x1A:
        {
            a = memory.readByte(getDE());
            programCounter += 1;
            break;
        }

        case 0x22:
        {
            memory.writeByte(getHL(), a);
            setHL(getHL() + 1);

            programCounter += 1;
            break;
        }

        case 0x2A:
        {
            a = memory.readByte(getHL());
            setHL(getHL() + 1);
            programCounter += 1;
            break;
        }

        case 0x3A:
        {
            a = memory.readByte(getHL());
            setHL(getHL() - 1);
            programCounter += 1;
            break;
        }

        case 0x86:
        {
            addRegister(memory.readByte(getHL()));
            break;
        }

        case 0x8E:
        {
            addWithCarry(memory.readByte(getHL()));
            break;
        }

        case 0x96:
        {
            subtractRegister(memory.readByte(getHL()));
            break;
        }

        case 0x9E:
        {
            subtractWithCarry(memory.readByte(getHL()));
            break;
        }

        case 0xA6:
        {
            andRegister(memory.readByte(getHL()));
            break;
        }

        case 0xAE:
        {
            uint8_t memVal = memory.readByte(getHL());
            xorRegister(memVal);
            break;
        }

        case 0xB6:
        {
            orRegister(memory.readByte(getHL()));
            break;
        }

        case 0xBE:
        {
            compareRegister(memory.readByte(getHL()));
            break;
        }

        //RST
        case 0x08:
        {
            uint16_t address = readWord();

            memory.writeByte(address,
                            stackPointer & 0xFF);

            memory.writeByte(address + 1,
                            (stackPointer >> 8) & 0xFF);

            programCounter += 3;
            break;
        }

        case 0x10:
        {
            stopped = true;

            programCounter += 2;
            break;
        }

        case 0x76:
        {
            halted = true;

            programCounter += 1;
            break;
        }

        case 0xD9:
        {
            returnFromSubroutine();
            ime = true;
            return 16;
            break;
        }

        case 0xE0:
        {
            uint16_t address =
                0xFF00 +
                memory.readByte(programCounter + 1);

            memory.writeByte(address, a);

            programCounter += 2;
            break;
        }

        case 0xE2:
        {
            memory.writeByte(0xFF00 + c, a);

            programCounter += 1;
            break;
        }

        case 0xE8: 
        {
            int8_t signedValue = static_cast<int8_t>(memory.readByte(programCounter + 1));
            uint8_t unsignedValue = memory.readByte(programCounter + 1);
            uint16_t result = stackPointer + signedValue;
            setFlag(flagZ, false);
            setFlag(flagN, false);
            setFlag(flagH, ((stackPointer & 0x0F) + (unsignedValue & 0x0F)) > 0x0F);
            setFlag(flagC, ((stackPointer & 0xFF) + unsignedValue) > 0xFF);
            stackPointer = result;
            programCounter += 2;
            break;
        }

        case 0xE9:
        {
            jump(getHL());
            break;
        }

        case 0xEA:
        {
            uint16_t address = readWord();

            memory.writeByte(address, a);

            programCounter += 3;
            break;
        }

        case 0xF0:
        {
            uint16_t address =
                0xFF00 +
                memory.readByte(programCounter + 1);

            a = memory.readByte(address);

            programCounter += 2;
            break;
        }

        case 0xF2:
        {
            a = memory.readByte(0xFF00 + c);

            programCounter += 1;
            break;
        }

        case 0xF3:
        {
            ime = false;

            programCounter += 1;
            break;
        }

        case 0xF8:
        {
            int8_t signedValue = static_cast<int8_t>(memory.readByte(programCounter + 1));
            uint8_t unsignedValue = memory.readByte(programCounter + 1);
            uint16_t result = stackPointer + signedValue;
            setFlag(flagZ, false);
            setFlag(flagN, false);
            setFlag(flagH, ((stackPointer & 0x0F) + (unsignedValue & 0x0F)) > 0x0F);
            setFlag(flagC, ((stackPointer & 0xFF) + unsignedValue) > 0xFF);
            setHL(result);
            programCounter += 2;
            break;
        }

        case 0xF9:
        {
            stackPointer = getHL();

            programCounter += 1;
            break;
        }

        case 0xFA:
        {
            uint16_t address = readWord();

            a = memory.readByte(address);

            programCounter += 3;
            break;
        }

        case 0xFB:
        {
            ime = true;

            programCounter += 1;
            break;
        }

        case 0xC7:
        {
            pushStack(programCounter + 1);
            jump(0x0000);
            break;
        }

        case 0xCF:
        {
            pushStack(programCounter + 1);
            jump(0x0008);
            break;
        }

        case 0xD7:
        {
            pushStack(programCounter + 1);
            jump(0x0010);
            break;
        }

        case 0xDF:
        {
            pushStack(programCounter + 1);
            jump(0x0018);
            break;
        }

        case 0xE7:
        {
            pushStack(programCounter + 1);
            jump(0x0020);
            break;
        }

        case 0xEF:
        {
            pushStack(programCounter + 1);
            jump(0x0028);
            break;
        }

        case 0xF7:
        {
            pushStack(programCounter + 1);
            jump(0x0030);
            break;
        }

        case 0xFF:
        {
            pushStack(programCounter + 1);
            jump(0x0038);
            break;
        }

        default:
        {
            std::cerr
                << "Unknown opcode: 0x"
                << std::hex
                << static_cast<int>(opcode)
                << '\n';

            break;
        }
    }
    return cycles;
}

uint8_t CPU::executeCBOpcode(uint8_t opcode)
{
    switch(opcode)
    {
        //RLC
        case 0x00:
            rotateLeftCircular(b);
            break;

        case 0x01:
            rotateLeftCircular(c);
            break;

        case 0x02:
            rotateLeftCircular(d);
            break;

        case 0x03:
            rotateLeftCircular(e);
            break;

        case 0x04:
            rotateLeftCircular(h);
            break;

        case 0x05:
            rotateLeftCircular(l);
            break;

        case 0x06:
        {
            uint8_t value = memory.readByte(getHL());
            rotateLeftCircular(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x07:
            rotateLeftCircular(a);
            break;

        //RRC
        case 0x08:
            rotateRightCircular(b);
            break;

        case 0x09:
            rotateRightCircular(c);
            break;

        case 0x0A:
            rotateRightCircular(d);
            break;

        case 0x0B:
            rotateRightCircular(e);
            break;

        case 0x0C:
            rotateRightCircular(h);
            break;

        case 0x0D:
            rotateRightCircular(l);
            break;

        case 0x0E:
        {
            uint8_t value = memory.readByte(getHL());
            rotateRightCircular(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x0F:
            rotateRightCircular(a);
            break;

        //RL
        case 0x10:
            rotateLeft(b);
            break;

        case 0x11:
            rotateLeft(c);
            break;

        case 0x12:
            rotateLeft(d);
            break;

        case 0x13:
            rotateLeft(e);
            break;

        case 0x14:
            rotateLeft(h);
            break;

        case 0x15:
            rotateLeft(l);
            break;

        case 0x16:
        {
            uint8_t value = memory.readByte(getHL());
            rotateLeft(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x17:
            rotateLeft(a);
            break;

        //RR
        case 0x18:
            rotateRight(b);
            break;

        case 0x19:
            rotateRight(c);
            break;

        case 0x1A:
            rotateRight(d);
            break;

        case 0x1B:
            rotateRight(e);
            break;

        case 0x1C:
            rotateRight(h);
            break;

        case 0x1D:
            rotateRight(l);
            break;

        case 0x1E:
        {
            uint8_t value = memory.readByte(getHL());
            rotateRight(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x1F:
            rotateRight(a);
            break;

        //SLA
        case 0x20:
            shiftLeftArithmetic(b);
            break;

        case 0x21:
            shiftLeftArithmetic(c);
            break;

        case 0x22:
            shiftLeftArithmetic(d);
            break;

        case 0x23:
            shiftLeftArithmetic(e);
            break;

        case 0x24:
            shiftLeftArithmetic(h);
            break;

        case 0x25:
            shiftLeftArithmetic(l);
            break;

        case 0x26:
        {
            uint8_t value = memory.readByte(getHL());
            shiftLeftArithmetic(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x27:
            shiftLeftArithmetic(a);
            break;

        //SRA
        case 0x28:
            shiftRightArithmetic(b);
            break;

        case 0x29:
            shiftRightArithmetic(c);
            break;

        case 0x2A:
            shiftRightArithmetic(d);
            break;

        case 0x2B:
            shiftRightArithmetic(e);
            break;

        case 0x2C:
            shiftRightArithmetic(h);
            break;

        case 0x2D:
            shiftRightArithmetic(l);
            break;

        case 0x2E:
        {
            uint8_t value = memory.readByte(getHL());
            shiftRightArithmetic(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x2F:
            shiftRightArithmetic(a);
            break;

        // SWAP - swap upper and lower nibbles
        case 0x30: { uint8_t r = b; b = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, b == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x31: { uint8_t r = c; c = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, c == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x32: { uint8_t r = d; d = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, d == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x33: { uint8_t r = e; e = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, e == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x34: { uint8_t r = h; h = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, h == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x35: { uint8_t r = l; l = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, l == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x36: { uint8_t value = memory.readByte(getHL()); value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4); memory.writeByte(getHL(), value); setFlag(flagZ, value == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }
        case 0x37: { uint8_t r = a; a = ((r & 0x0F) << 4) | ((r & 0xF0) >> 4); setFlag(flagZ, a == 0); setFlag(flagN, false); setFlag(flagH, false); setFlag(flagC, false); break; }

        //SRL
        case 0x38:
            shiftRightLogical(b);
            break;

        case 0x39:
            shiftRightLogical(c);
            break;

        case 0x3A:
            shiftRightLogical(d);
            break;

        case 0x3B:
            shiftRightLogical(e);
            break;

        case 0x3C:
            shiftRightLogical(h);
            break;

        case 0x3D:
            shiftRightLogical(l);
            break;

        case 0x3E:
        {
            uint8_t value = memory.readByte(getHL());
            shiftRightLogical(value);
            memory.writeByte(getHL(), value);
            break;
        }

        case 0x3F:
            shiftRightLogical(a);
            break;

        //BIT 0
        case 0x40: testBit(0, b); break;
        case 0x41: testBit(0, c); break;
        case 0x42: testBit(0, d); break;
        case 0x43: testBit(0, e); break;
        case 0x44: testBit(0, h); break;
        case 0x45: testBit(0, l); break;
        case 0x46: testBit(0, memory.readByte(getHL())); break;
        case 0x47: testBit(0, a); break;

        //BIT 1
        case 0x48: testBit(1, b); break;
        case 0x49: testBit(1, c); break;
        case 0x4A: testBit(1, d); break;
        case 0x4B: testBit(1, e); break;
        case 0x4C: testBit(1, h); break;
        case 0x4D: testBit(1, l); break;
        case 0x4E: testBit(1, memory.readByte(getHL())); break;
        case 0x4F: testBit(1, a); break;

        //BIT2
        case 0x50: testBit(2, b); break;
        case 0x51: testBit(2, c); break;
        case 0x52: testBit(2, d); break;
        case 0x53: testBit(2, e); break;
        case 0x54: testBit(2, h); break;
        case 0x55: testBit(2, l); break;
        case 0x56: testBit(2, memory.readByte(getHL())); break;
        case 0x57: testBit(2, a); break;

        //BIT 3
        case 0x58: testBit(3, b); break;
        case 0x59: testBit(3, c); break;
        case 0x5A: testBit(3, d); break;
        case 0x5B: testBit(3, e); break;
        case 0x5C: testBit(3, h); break;
        case 0x5D: testBit(3, l); break;
        case 0x5E: testBit(3, memory.readByte(getHL())); break;
        case 0x5F: testBit(3, a); break;

        //BIT 4
        case 0x60: testBit(4, b); break;
        case 0x61: testBit(4, c); break;
        case 0x62: testBit(4, d); break;
        case 0x63: testBit(4, e); break;
        case 0x64: testBit(4, h); break;
        case 0x65: testBit(4, l); break;
        case 0x66: testBit(4, memory.readByte(getHL())); break;
        case 0x67: testBit(4, a); break;

        //BIT 5
        case 0x68: testBit(5, b); break;
        case 0x69: testBit(5, c); break;
        case 0x6A: testBit(5, d); break;
        case 0x6B: testBit(5, e); break;
        case 0x6C: testBit(5, h); break;
        case 0x6D: testBit(5, l); break;
        case 0x6E: testBit(5, memory.readByte(getHL())); break;
        case 0x6F: testBit(5, a); break;

        //BIT 6
        case 0x70: testBit(6, b); break;
        case 0x71: testBit(6, c); break;
        case 0x72: testBit(6, d); break;
        case 0x73: testBit(6, e); break;
        case 0x74: testBit(6, h); break;
        case 0x75: testBit(6, l); break;
        case 0x76: testBit(6, memory.readByte(getHL())); break;
        case 0x77: testBit(6, a); break;

        //BIT 7
        case 0x78: testBit(7, b); break;
        case 0x79: testBit(7, c); break;
        case 0x7A: testBit(7, d); break;
        case 0x7B: testBit(7, e); break;
        case 0x7C: testBit(7, h); break;
        case 0x7D: testBit(7, l); break;
        case 0x7E: testBit(7, memory.readByte(getHL())); break;
        case 0x7F: testBit(7, a); break;

        //RES 0
        case 0x80: resetBit(0, b); break;
        case 0x81: resetBit(0, c); break;
        case 0x82: resetBit(0, d); break;
        case 0x83: resetBit(0, e); break;
        case 0x84: resetBit(0, h); break;
        case 0x85: resetBit(0, l); break;
        case 0x86:
        {
            uint8_t value = memory.readByte(getHL());

            resetBit(0, value);

            memory.writeByte(getHL(), value);
            break;
        }
        case 0x87: resetBit(0, a); break;

        //RES 1
        case 0x88: resetBit(1, b); break;
        case 0x89: resetBit(1, c); break;
        case 0x8A: resetBit(1, d); break;
        case 0x8B: resetBit(1, e); break;
        case 0x8C: resetBit(1, h); break;
        case 0x8D: resetBit(1, l); break;
        case 0x8E:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(1, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0x8F: resetBit(1, a); break;

        //RES 2
        case 0x90: resetBit(2, b); break;
        case 0x91: resetBit(2, c); break;
        case 0x92: resetBit(2, d); break;
        case 0x93: resetBit(2, e); break;
        case 0x94: resetBit(2, h); break;
        case 0x95: resetBit(2, l); break;
        case 0x96:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(2, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0x97: resetBit(2, a); break;

        //RES 3
        case 0x98: resetBit(3, b); break;
        case 0x99: resetBit(3, c); break;
        case 0x9A: resetBit(3, d); break;
        case 0x9B: resetBit(3, e); break;
        case 0x9C: resetBit(3, h); break;
        case 0x9D: resetBit(3, l); break;
        case 0x9E:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(3, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0x9F: resetBit(3, a); break;

        //RES 4
        case 0xA0: resetBit(4, b); break;
        case 0xA1: resetBit(4, c); break;
        case 0xA2: resetBit(4, d); break;
        case 0xA3: resetBit(4, e); break;
        case 0xA4: resetBit(4, h); break;
        case 0xA5: resetBit(4, l); break;
        case 0xA6:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(4, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xA7: resetBit(4, a); break;

        //RES 5
        case 0xA8: resetBit(5, b); break;
        case 0xA9: resetBit(5, c); break;
        case 0xAA: resetBit(5, d); break;
        case 0xAB: resetBit(5, e); break;
        case 0xAC: resetBit(5, h); break;
        case 0xAD: resetBit(5, l); break;
        case 0xAE:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(5, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xAF: resetBit(5, a); break;

        //RES 6
        case 0xB0: resetBit(6, b); break;
        case 0xB1: resetBit(6, c); break;
        case 0xB2: resetBit(6, d); break;
        case 0xB3: resetBit(6, e); break;
        case 0xB4: resetBit(6, h); break;
        case 0xB5: resetBit(6, l); break;
        case 0xB6:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(6, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xB7: resetBit(6, a); break;

        //RES 7
        case 0xB8: resetBit(7, b); break;
        case 0xB9: resetBit(7, c); break;
        case 0xBA: resetBit(7, d); break;
        case 0xBB: resetBit(7, e); break;
        case 0xBC: resetBit(7, h); break;
        case 0xBD: resetBit(7, l); break;
        case 0xBE:
        {
            uint8_t value = memory.readByte(getHL());
            resetBit(7, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xBF: resetBit(7, a); break;

        //SET 0
        case 0xC0: setBit(0, b); break;
        case 0xC1: setBit(0, c); break;
        case 0xC2: setBit(0, d); break;
        case 0xC3: setBit(0, e); break;
        case 0xC4: setBit(0, h); break;
        case 0xC5: setBit(0, l); break;
        case 0xC6:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(0, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xC7: setBit(0, a); break;

        //SET 1
        case 0xC8: setBit(1, b); break;
        case 0xC9: setBit(1, c); break;
        case 0xCA: setBit(1, d); break;
        case 0xCB: setBit(1, e); break;
        case 0xCC: setBit(1, h); break;
        case 0xCD: setBit(1, l); break;
        case 0xCE:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(1, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xCF: setBit(1, a); break;

        //SET 2
        case 0xD0: setBit(2, b); break;
        case 0xD1: setBit(2, c); break;
        case 0xD2: setBit(2, d); break;
        case 0xD3: setBit(2, e); break;
        case 0xD4: setBit(2, h); break;
        case 0xD5: setBit(2, l); break;
        case 0xD6:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(2, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xD7: setBit(2, a); break;

        //SET 3
        case 0xD8: setBit(3, b); break;
        case 0xD9: setBit(3, c); break;
        case 0xDA: setBit(3, d); break;
        case 0xDB: setBit(3, e); break;
        case 0xDC: setBit(3, h); break;
        case 0xDD: setBit(3, l); break;
        case 0xDE:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(3, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xDF: setBit(3, a); break;

        //SET 4
        case 0xE0: setBit(4, b); break;
        case 0xE1: setBit(4, c); break;
        case 0xE2: setBit(4, d); break;
        case 0xE3: setBit(4, e); break;
        case 0xE4: setBit(4, h); break;
        case 0xE5: setBit(4, l); break;
        case 0xE6:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(4, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xE7: setBit(4, a); break;

        //SET 5
        case 0xE8: setBit(5, b); break;
        case 0xE9: setBit(5, c); break;
        case 0xEA: setBit(5, d); break;
        case 0xEB: setBit(5, e); break;
        case 0xEC: setBit(5, h); break;
        case 0xED: setBit(5, l); break;
        case 0xEE:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(5, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xEF: setBit(5, a); break;

        //SET 6
        case 0xF0: setBit(6, b); break;
        case 0xF1: setBit(6, c); break;
        case 0xF2: setBit(6, d); break;
        case 0xF3: setBit(6, e); break;
        case 0xF4: setBit(6, h); break;
        case 0xF5: setBit(6, l); break;
        case 0xF6:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(6, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xF7: setBit(6, a); break;

        //SET 7
        case 0xF8: setBit(7, b); break;
        case 0xF9: setBit(7, c); break;
        case 0xFA: setBit(7, d); break;
        case 0xFB: setBit(7, e); break;
        case 0xFC: setBit(7, h); break;
        case 0xFD: setBit(7, l); break;
        case 0xFE:
        {
            uint8_t value = memory.readByte(getHL());
            setBit(7, value);
            memory.writeByte(getHL(), value);
            break;
        }
        case 0xFF: setBit(7, a); break;

    }

    return cbOpcodeCycles[opcode];
}

bool CPU::isHalted() const
{
    return halted;
}

bool CPU::isStopped() const
{
    return stopped;
}

bool CPU::getIME() const
{
    return ime;
}
