#include "APU.h"
#include "Memory.h"
#include <iostream>

APU::APU(Memory& memory) : memory(memory)
{
    SDL_AudioSpec spec{};
    spec.freq     = sampleRate;
    spec.format   = SDL_AUDIO_F32;
    spec.channels = 2;  // stereo!

    audioStream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        nullptr,
        nullptr);

    if(audioStream)
    {
        std::cerr << "SDL Audio opened successfully!\n";
        SDL_ResumeAudioStreamDevice(audioStream);
    }
    else
    {
        std::cerr << "SDL Audio FAILED: " << SDL_GetError() << "\n";
    }
}

APU::~APU()
{
    if(audioStream)
        SDL_DestroyAudioStream(audioStream);
}

void APU::update(uint16_t cycles)
{
    if((memory.readByte(0xFF26) & 0x80) == 0)
        return;

    frameSequencerCounter += cycles;
    while(frameSequencerCounter >= 8192)
    {
        frameSequencerCounter -= 8192;
        stepFrameSequencer();
    }

    updateChannel1(cycles);
    updateChannel2(cycles);
    updateChannel3(cycles);
    updateChannel4(cycles);

    currentSample = mixAudio();
    updateStatusRegister();

    sampleCounter += cycles;
    constexpr uint32_t cyclesPerSample = cpuClock / sampleRate;

    while(sampleCounter >= cyclesPerSample)
    {
        sampleCounter -= cyclesPerSample;

        float stereo[2] = {currentSample.left, currentSample.right};

        if(audioStream)
            SDL_PutAudioStreamData(audioStream, stereo, sizeof(stereo));
    }
}

void APU::writeRegister(uint16_t address, uint8_t value)
{
    if(address == 0xFF26) return;

    switch(address)
    {
        case 0xFF14:
            if(value & 0x80) triggerChannel1();
            break;
        case 0xFF19:
            if(value & 0x80) triggerChannel2();
            break;
        case 0xFF1E:
            if(value & 0x80) triggerChannel3();
            break;
        case 0xFF23:
            if(value & 0x80) triggerChannel4();
            break;
    }
}

void APU::stepFrameSequencer()
{
    switch(frameSequencerStep)
    {
        case 0:
        case 2:
        case 4:
        case 6:
            clockLengthCounters();
            break;

        case 7:
            clockEnvelopes();
            break;
    }

    if(frameSequencerStep == 2 || frameSequencerStep == 6)
        clockSweep();

    frameSequencerStep = (frameSequencerStep + 1) & 7;
}

void APU::clockLengthCounters()
{
    clockSquareLength(
        channel1.square,
        readLengthEnable(0xFF14));

    clockSquareLength(
        channel2.square,
        readLengthEnable(0xFF19));

    clockWaveLength();
}

void APU::clockEnvelopes()
{
    clockSquareEnvelope(channel1.square,0xFF12);
    clockSquareEnvelope(channel2.square,0xFF17);
    clockNoiseEnvelope();
}

void APU::clockSweep()
{
    if(!channel1.square.enabled)
        return;

    uint8_t nr10 = memory.readByte(0xFF10);
    uint8_t period = (nr10 >> 4) & 0x07;
    if(period == 0)
        return;
    if(channel1.sweepTimer > 0)
        channel1.sweepTimer--;
    if(channel1.sweepTimer != 0)
        return;
    channel1.sweepTimer = period;
    uint16_t frequency = readFrequency(0xFF13,0xFF14);
    uint8_t shift = nr10 & 0x07;
    uint16_t delta = frequency >> shift;
    if(nr10 & 0x08)
        frequency -= delta;
    else
        frequency += delta;

    if(frequency > 2047)
    {
        channel1.square.enabled = false;
        return;
    }
    memory.writeByte(0xFF13,frequency & 0xFF);
    uint8_t nr14 = memory.readByte(0xFF14);
    nr14 = (nr14 & 0xF8) | ((frequency >> 8) & 0x07);
    memory.writeByte(0xFF14,nr14);
}

void APU::updateChannel1(uint16_t cycles)
{
    if(!channel1.square.enabled) return;
    if(!isDAC1Enabled())
    {
        channel1.square.enabled = false;
        return;
    }

    uint16_t frequency = readFrequency(0xFF13, 0xFF14);
    clockSquareTimer(channel1.square, cycles, frequency);
    uint8_t duty = readDuty(0xFF11);
    channel1.square.output = getSquareOutput(channel1.square, duty);
}

void APU::updateChannel2(uint16_t cycles)
{
    if(!channel2.square.enabled) return;
    if(!isDAC2Enabled())
    {
        channel2.square.enabled = false;
        return;
    }

    uint16_t frequency = readFrequency(0xFF18, 0xFF19);
    clockSquareTimer(channel2.square, cycles, frequency);
    uint8_t duty = readDuty(0xFF16);
    channel2.square.output = getSquareOutput(channel2.square, duty);
}

void APU::updateChannel3(uint16_t cycles)
{
    if (!channel3.enabled || !isDACEnabled(0xFF1A)) return;

    if (channel3.frequencyTimer == 0)
    {
        uint16_t frequency = readFrequency(0xFF1D, 0xFF1E);
        channel3.frequencyTimer = (2048 - frequency) * 2;
    }

    clockWaveTimer(cycles);

    channel3.output = getWaveOutput();
}

void APU::updateChannel4(uint16_t cycles)
{
    if(!channel4.enabled) return;
    if(!isDAC4Enabled())
    {
        channel4.enabled = false;
        return;
    }

    if(channel4.frequencyTimer == 0)
    {
        static constexpr uint8_t divisors[8] = {8,16,32,48,64,80,96,112};
        uint8_t nr43 = memory.readByte(0xFF22);
        uint8_t divisorCode = nr43 & 0x07;
        uint8_t clockShift = (nr43 >> 4) & 0x0F;
        channel4.frequencyTimer = divisors[divisorCode] << clockShift;
    }

    clockNoiseTimer(cycles);
    channel4.output = getNoiseOutput();
}


void APU::triggerChannel1()
{
    channel1.square.enabled = true;
    if(channel1.square.lengthCounter == 0)
        channel1.square.lengthCounter = 64;

    uint16_t frequency = readFrequency(0xFF13,0xFF14);
    channel1.square.frequencyTimer = (2048-frequency) * 4;
    channel1.square.wavePosition = 0;
    uint8_t duty = readDuty(0xFF11);   
    channel1.square.output = getSquareOutput(channel1.square,duty);
    channel1.square.currentVolume = readInitialVolume(0xFF12);
    channel1.square.envelopeTimer = readEnvelopePeriod(0xFF12);
    uint8_t nr10 = memory.readByte(0xFF10);
    channel1.sweepTimer = (nr10 >> 4) & 0x07;
}

void APU::triggerChannel2()
{
    channel2.square.enabled = true;
    if(channel2.square.lengthCounter == 0)
        channel2.square.lengthCounter = 64;

    uint16_t frequency = readFrequency(0xFF18,0xFF19);
    channel2.square.frequencyTimer = (2048-frequency) * 4;
    channel2.square.wavePosition = 0;
    uint8_t duty = readDuty(0xFF16);
    channel2.square.output = getSquareOutput(channel2.square,duty);
    channel2.square.currentVolume = readInitialVolume(0xFF17);
    channel2.square.envelopeTimer = readEnvelopePeriod(0xFF17);
}

void APU::triggerChannel3()
{
    channel3.enabled = true;
    if(channel3.lengthCounter == 0)
        channel3.lengthCounter = 256;

    uint16_t frequency = readFrequency(0xFF1D,0xFF1E);
    channel3.frequencyTimer = (2048-frequency) * 2;
    channel3.wavePosition = 0;
    channel3.output = getWaveOutput();
}

void APU::triggerChannel4()
{
    channel4.enabled = true;

    if(channel4.lengthCounter == 0)
        channel4.lengthCounter = 64;

    channel4.lfsr = 0x7FFF;

    channel4.currentVolume = readInitialVolume(0xFF21);
    channel4.envelopeTimer = readEnvelopePeriod(0xFF21);
}

void APU::clockSquareTimer(SquareChannelRuntime& channel, uint16_t cycles, uint16_t frequency)
{
    if(channel.frequencyTimer > cycles)
    {
        channel.frequencyTimer -= cycles;
        return;
    }

    // timer expired - advance wave position and reload
    cycles -= channel.frequencyTimer;
    channel.wavePosition = (channel.wavePosition + 1) & 7;
    channel.frequencyTimer = (2048 - frequency) * 4;

    // consume remaining cycles
    if(channel.frequencyTimer > cycles)
    {
        channel.frequencyTimer -= cycles;
    }
    else
    {
        channel.frequencyTimer = 0;
    }
}

void APU::clockWaveTimer(uint16_t cycles)
{
    if(channel3.frequencyTimer > cycles)
    {
        channel3.frequencyTimer -= cycles;
        return;
    }

    channel3.frequencyTimer = 0;
    channel3.wavePosition = (channel3.wavePosition + 1) & 31;
}

void APU::clockNoiseTimer(uint16_t cycles)
{
    if(channel4.frequencyTimer > cycles)
    {
        channel4.frequencyTimer -= cycles;
        return;
    }

    channel4.frequencyTimer = 0;

    uint8_t nr43 = memory.readByte(0xFF22);

    bool widthMode = (nr43 & 0x08) != 0;

    uint8_t feedback =
        (channel4.lfsr & 1) ^
        ((channel4.lfsr >> 1) & 1);

    channel4.lfsr >>= 1;
    channel4.lfsr |= feedback << 14;

    if(widthMode)
    {
        channel4.lfsr &= ~(1 << 6);
        channel4.lfsr |= feedback << 6;
    }
}

void APU::clockSquareLength(SquareChannelRuntime& channel,bool enabled)
{
    if(!enabled)
        return;

    if(channel.lengthCounter == 0)
        return;

    channel.lengthCounter--;

    if(channel.lengthCounter == 0)
        channel.enabled = false;
}

void APU::clockWaveLength()
{
    if(!readLengthEnable(0xFF1E))
        return;

    if(channel3.lengthCounter == 0)
        return;

    channel3.lengthCounter--;

    if(channel3.lengthCounter == 0)
        channel3.enabled = false;
}

void APU::clockNoiseLength()
{
    if(!readLengthEnable(0xFF23))
        return;

    if(channel4.lengthCounter == 0)
        return;

    channel4.lengthCounter--;

    if(channel4.lengthCounter == 0)
        channel4.enabled = false;
}

void APU::clockSquareEnvelope(SquareChannelRuntime& channel,uint16_t nrx2)
{
    clockGenericEnvelope(
        channel.currentVolume,
        channel.envelopeTimer,
        nrx2
    );
}

void APU::clockNoiseEnvelope()
{
    clockGenericEnvelope(
        channel4.currentVolume,
        channel4.envelopeTimer,
        0xFF21
    );
}

void APU::clockGenericEnvelope(uint8_t& currentVolume,uint8_t& envelopeTimer,uint16_t nrx2)
{
    uint8_t period = readEnvelopePeriod(nrx2);

    if(period == 0)
        return;

    if(envelopeTimer > 0)
        envelopeTimer--;

    if(envelopeTimer != 0)
        return;

    envelopeTimer = period;

    if(readEnvelopeDirection(nrx2))
    {
        if(currentVolume < 15)
            currentVolume++;
    }
    else
    {
        if(currentVolume > 0)
            currentVolume--;
    }
}

uint8_t APU::getSquareOutput(const SquareChannelRuntime& channel,uint8_t duty) const
{
    static constexpr uint8_t dutyTable[4][8] =
    {
        {0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,1,1},
        {0,1,1,1,1,1,1,0}
    };

    return dutyTable[duty][channel.wavePosition];
}

uint8_t APU::getWaveOutput() const
{
    uint8_t waveByte =
        memory.readByte(0xFF30 + (channel3.wavePosition >> 1));

    uint8_t sample;

    if(channel3.wavePosition & 1)
        sample = waveByte & 0x0F;
    else
        sample = waveByte >> 4;

    switch((memory.readByte(0xFF1C) >> 5) & 0x03)
    {
        case 0:
            return 0;

        case 1:
            return sample;

        case 2:
            return sample >> 1;

        case 3:
            return sample >> 2;
    }

    return 0;
}

uint8_t APU::getNoiseOutput() const
{
    return (~channel4.lfsr) & 1;
}

APU::StereoSample APU::mixAudio() const
{
    uint8_t nr50 = memory.readByte(0xFF24);
    uint8_t nr51 = memory.readByte(0xFF25);

    uint8_t leftVol  = ((nr50 >> 4) & 0x07) + 1;  
    uint8_t rightVol = (nr50 & 0x07) + 1;         

    int16_t leftSample  = 0;
    int16_t rightSample = 0;

    if(channel1.square.enabled)
    {
        uint8_t out = channel1.square.output * channel1.square.currentVolume;
        if(nr51 & 0x10) leftSample  += out;
        if(nr51 & 0x01) rightSample += out;
    }

    if(channel2.square.enabled)
    {
        uint8_t out = channel2.square.output * channel2.square.currentVolume;
        if(nr51 & 0x20) leftSample  += out;
        if(nr51 & 0x02) rightSample += out;
    }

    if(channel3.enabled)
    {
        uint8_t out = channel3.output;
        if(nr51 & 0x40) leftSample  += out;
        if(nr51 & 0x04) rightSample += out;
    }

    if(channel4.enabled)
    {
        uint8_t out = channel4.output * channel4.currentVolume;
        if(nr51 & 0x80) leftSample  += out;
        if(nr51 & 0x08) rightSample += out;
    }
    float left  = (leftSample  * leftVol)  / (240.0f * 8.0f);
    float right = (rightSample * rightVol) / (240.0f * 8.0f);

    // Clamp
    if(left  >  1.0f) left  =  1.0f;
    if(left  < -1.0f) left  = -1.0f;
    if(right >  1.0f) right =  1.0f;
    if(right < -1.0f) right = -1.0f;

    return {left, right};
}

uint16_t APU::readFrequency(uint16_t lowRegister,uint16_t highRegister) const
{
    return memory.readByte(lowRegister) |
           ((memory.readByte(highRegister) & 0x07) << 8);
}

uint8_t APU::readDuty(uint16_t nrx1) const
{
    return (memory.readByte(nrx1) >> 6) & 0x03;
}

uint8_t APU::readInitialVolume(uint16_t nrx2) const
{
    return (memory.readByte(nrx2) >> 4) & 0x0F;
}

uint8_t APU::readEnvelopePeriod(uint16_t nrx2) const
{
    return memory.readByte(nrx2) & 0x07;
}

bool APU::readEnvelopeDirection(uint16_t nrx2) const
{
    return (memory.readByte(nrx2) & 0x08) != 0;
}

bool APU::readLengthEnable(uint16_t nrx4) const
{
    return (memory.readByte(nrx4) & 0x40) != 0;
}

bool APU::isTriggerSet(uint16_t nrx4) const
{
    return (memory.readByte(nrx4) & 0x80) != 0;
}

bool APU::isDACEnabled(uint16_t nrx0) const
{
    return (memory.readByte(nrx0) & 0x80) != 0;
}

void APU::updateStatusRegister()
{
    uint8_t nr52 = memory.readByte(0xFF26);
    nr52 &= 0xF0;

    if(channel1.square.enabled)
        nr52 |= 0x01;

    if(channel2.square.enabled)
        nr52 |= 0x02;

    if(channel3.enabled)
        nr52 |= 0x04;

    if(channel4.enabled)
        nr52 |= 0x08;

    memory.writeByte(0xFF26,nr52);
}

void APU::checkTriggers()
{
    if(isTriggerSet(0xFF14))
    {
        triggerChannel1();
        memory.writeByte(0xFF14, memory.readByte(0xFF14) & ~0x80);
    }

    if(isTriggerSet(0xFF19))
    {
        triggerChannel2();
        memory.writeByte(0xFF19, memory.readByte(0xFF19) & ~0x80);
    }

    if(isTriggerSet(0xFF1E))
    {
        triggerChannel3();
        memory.writeByte(0xFF1E, memory.readByte(0xFF1E) & ~0x80);
    }

    if(isTriggerSet(0xFF23))
    {
        triggerChannel4();
        memory.writeByte(0xFF23, memory.readByte(0xFF23) & ~0x80);
    }
}

bool APU::isDAC1Enabled() const
{
    return (memory.readByte(0xFF12) & 0xF8) != 0;
}

bool APU::isDAC2Enabled() const
{
    return (memory.readByte(0xFF17) & 0xF8) != 0;
}

bool APU::isDAC4Enabled() const
{
    return (memory.readByte(0xFF21) & 0xF8) != 0;
}