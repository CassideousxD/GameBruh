#include "Memory.h"
#include <algorithm>
#include <iostream>
#include <fstream>

Memory::Memory()
{
    std::fill(std::begin(memory), std::end(memory), 0);
    cartridge = nullptr;
    bootRomEnabled = true;
    std::ifstream bootFile("../bootstrap/dmg_boot.bin", std::ios::binary);
    if (bootFile)
    {
        bootFile.read(reinterpret_cast<char*>(bootRom), 256);
        std::cout << "Boot ROM loaded successfully!" << std::endl;
    }
    else
    {
        std::cerr << "Warning: dmg_boot.bin not found! Defaulting to game skip." << std::endl;
        bootRomEnabled = false; 
    }
    interruptEnable = 0x00;
    interruptFlag = 0x00;
    divRegister = 0x00;
    tima = 0x00;
    tma = 0x00;
    tac = 0x00;
    ly = 0;
    scx = 0;
    scy = 0;
    serialData = 0x00;
    serialControl = 0x00;
    buttonMatrix[0] = 0x0F;
    buttonMatrix[1] = 0x0F;
    joypadSelect = 0x30;
    initializeHardwareRegisters();
}

void Memory::initializeHardwareRegisters()
{
    // Timer
    memory[0xFF05] = 0x00; // TIMA
    memory[0xFF06] = 0x00; // TMA
    memory[0xFF07] = 0x00; // TAC

    // Sound
    memory[0xFF10] = 0x80;
    memory[0xFF11] = 0xBF;
    memory[0xFF12] = 0xF3;
    memory[0xFF14] = 0xBF;

    memory[0xFF16] = 0x3F;
    memory[0xFF17] = 0x00;
    memory[0xFF19] = 0xBF;

    memory[0xFF1A] = 0x7F;
    memory[0xFF1B] = 0xFF;
    memory[0xFF1C] = 0x9F;
    memory[0xFF1E] = 0xBF;

    memory[0xFF20] = 0xFF;
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF;

    memory[0xFF24] = 0x77;
    memory[0xFF25] = 0xF3;
    memory[0xFF26] = 0xF1;

    // LCD
    memory[0xFF40] = 0x91; // LCDC
    lcdc = 0x91;
    memory[0xFF41] = 0x85; // STAT
    memory[0xFF42] = 0x00; // SCY
    memory[0xFF43] = 0x00; // SCX
    memory[0xFF44] = 0x00; // LY
    memory[0xFF45] = 0x00; // LYC

    // Palettes
    memory[0xFF47] = 0xFC;
    memory[0xFF48] = 0xFF;
    memory[0xFF49] = 0xFF;

    // Window
    memory[0xFF4A] = 0x00;
    memory[0xFF4B] = 0x00;

    wy = 0x00;
    wx = 0x00;

    // Interrupts
    memory[0xFF0F] = 0x00; // IF
    memory[0xFFFF] = 0x00; // IE
}

void Memory::connectCartridge(Cartridge* cartridge)
{
    this->cartridge = cartridge;
}

uint8_t Memory::readByte(uint16_t address) const
{

    if (bootRomEnabled && address < 0x0100)
    {
        return bootRom[address];
    }

    if(address == 0xFF00)
    {
        return joypad.read();
    }

    if(address < 0x8000 && cartridge != nullptr)
    {
        return cartridge->readByte(address);
    }

    if(address == 0xFF01)
    {
        return serialData;
    }

    if(address == 0xFF02)
    {
        return serialControl;
    }

    if(address >= 0xFEA0 && address <= 0xFEFF)
    {
        return 0xFF;
    }

    if(address == 0xFFFF)
    {
        return interruptEnable;
    }

    if(address == 0xFF42)
    {
        return scy;
    }

    if(address == 0xFF43)
    {
        return scx;
    }
    if(address == 0xFF4A)
    {
        return wy;
    }

    if(address == 0xFF4B)
    {
        return wx;
    }
    if(address == 0xFF0F)
    {
        return interruptFlag;
    }
    if(address == 0xFF04)
    {
        return divRegister;
    }
    if(address == 0xFF05)
    {
        return tima;
    }
    if(address == 0xFF44)
    {
        return ly;
    }

    if(address == 0xFF40)
    {
        return lcdc;
    }

    if(address == 0xFF46)
    {
        return dma;
    }

    if(address == 0xFF06)
    {
        return tma;
    }

    if(address == 0xFF07)
    {
        return tac;
    }

    if(address >= 0xE000 && address <= 0xFDFF)
    {
        return memory[address - 0x2000];
    }

    return memory[address];
}

void Memory::writeByte(uint16_t address, uint8_t value)
{
    if(address <= 0x7FFF)
    {
        return;
    }

    if (address == 0xFF50)
    {
        if (value != 0x00)
        {
            bootRomEnabled = false; 
            std::cout << "Boot ROM unmapped. Control handed over to Cartridge!" << std::endl;
        }
        return;
    }

    if(address == 0xFF00)
    {
        joypad.write(value);
        return;
    }

    if(address == 0xFF01)
    {
        serialData = value;
        return;
    }

    if(address == 0xFF02)
    {
        serialControl = value;
        if(serialControl == 0x81)
        {
            std::cout << static_cast<char>(serialData) << std::flush;
            serialData = 0xFF;      
            serialControl &= ~0x80;  
            interruptFlag |= 0x08;  
        }
        return;
    }

    if(address >= 0xFEA0 && address <= 0xFEFF)
    {
        return;
    }

    if(address == 0xFFFF)
    {
        interruptEnable = value;
        return;
    }

    if(address == 0xFF42)
    {
        scy = value;
        return;
    }

    if(address == 0xFF43)
    {
        scx = value;
        return;
    }

    if(address == 0xFF4A)
    {
        wy = value;
        return;
    }

    if(address == 0xFF4B)
    {
        wx = value;
        return;
    }

    if(address == 0xFF44)
    {
        ly = 0;
        return;
    }

    if(address == 0xFF46)
    {
        dma = value;
        doDMATransfer(value);
        return;
    }

    if(address == 0xFF04)
    {
        divRegister = 0x00;
        return;
    }

 if(address == 0xFF40)
    {
        lcdc = value;
        return;
    }

    if(address == 0xFF05)
    {
        tima = value;
        return;
    }

    if(address == 0xFF06)
    {
        tma = value;
        return;
    }

    if(address == 0xFF07)
    {
        tac = value;
        return;
    }

    if(address == 0xFF0F)
    {
        interruptFlag = value;
        return;
    }

    if(address >= 0xC000 && address <= 0xDDFF)
    {
        memory[address] = value;
        memory[address + 0x2000] = value;
        return;
    }

    if(address >= 0xE000 && address <= 0xFDFF)
    {
        memory[address] = value;
        memory[address - 0x2000] = value;
        return;
    }

    memory[address] = value;
}

void Memory::doDMATransfer(uint8_t value)
{
    uint16_t sourceAddress = value << 8;
    for (uint16_t i = 0; i < 0xA0; i++)
    {
        writeByte(0xFE00 + i, readByte(sourceAddress + i));
    }
}

void Memory::pressButton(Button button)
{
    joypad.press(button);
    if(joypad.hasInterruptRequest())
    {
        interruptFlag |= 0x10; // Trigger Joypad Interrupt
        joypad.clearInterruptRequest();
    }
}

void Memory::releaseButton(Button button)
{
    joypad.release(button);
}


void Memory::incrementDIV()
{
    divRegister++;
}

void Memory::requestTimerInterrupt()
{
    interruptFlag |= 0x04;
}

uint8_t Memory::getLY() const
{
    return ly;
}

void Memory::setLY(uint8_t value)
{
    ly = value;
}

void Memory::requestVBlankInterrupt()
{
    interruptFlag |= 0x01;
}

void Memory::requestSTATInterrupt()
{
    interruptFlag |= 0x02;
}

uint8_t Memory::getSCX() const
{
    return scx;
}

uint8_t Memory::getSCY() const
{
    return scy;
}

void Memory::setSCX(uint8_t value)
{
    scx = value;
}

void Memory::setSCY(uint8_t value)
{
    scy = value;
}

uint8_t Memory::getWX() const
{
    return wx;
}

uint8_t Memory::getWY() const
{
    return wy;
}

uint8_t Memory::getLCDC() const
{
    return lcdc;
}

void Memory::setWX(uint8_t value)
{
    wx = value;
}

void Memory::setWY(uint8_t value)
{
    wy = value;
}