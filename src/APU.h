#pragma once
#include <cstdint>
#include <SDL3/SDL.h>

class Memory;

class APU
{
public:
    explicit APU(Memory& memory);
    ~APU();
    void update(uint16_t cycles);
    void writeRegister(uint16_t address,uint8_t value);

private:

    SDL_AudioStream* audioStream = nullptr;
    static constexpr int sampleRate = 44100;
    static constexpr int cpuClock = 4194304;
    uint32_t sampleCounter = 0;

    struct SquareChannelRuntime
    {
        bool enabled = false;

        uint16_t frequencyTimer = 0;

        uint8_t wavePosition = 0;
        uint8_t output = 0;

        uint8_t lengthCounter = 0;

        uint8_t currentVolume = 0;
        uint8_t envelopeTimer = 0;
    };

    struct Channel1Runtime
    {
        SquareChannelRuntime square;

        uint8_t sweepTimer = 0;
    };

    struct Channel2Runtime
    {
        SquareChannelRuntime square;
    };

    struct WaveChannelRuntime
    {
        bool enabled = false;

        uint16_t frequencyTimer = 0;

        uint16_t lengthCounter = 0;

        uint8_t wavePosition = 0;
        uint8_t output = 0;
    };

    struct NoiseChannelRuntime
    {
        bool enabled = false;

        uint16_t lfsr = 0x7FFF;

        uint16_t frequencyTimer = 0;

        uint8_t lengthCounter = 0;

        uint8_t currentVolume = 0;
        uint8_t envelopeTimer = 0;

        uint8_t output = 0;
    };

    struct StereoSample
    {
        float left;
        float right;
    };

    Memory& memory;

    Channel1Runtime channel1;
    Channel2Runtime channel2;
    WaveChannelRuntime channel3;
    NoiseChannelRuntime channel4;

    uint32_t frameSequencerCounter = 0;
    uint8_t frameSequencerStep = 0;

    StereoSample currentSample = {0.0f, 0.0f};

    void stepFrameSequencer();

    void clockLengthCounters();
    void clockEnvelopes();
    void clockSweep();

    void updateChannel1(uint16_t cycles);
    void updateChannel2(uint16_t cycles);
    void updateChannel3(uint16_t cycles);
    void updateChannel4(uint16_t cycles);

    void triggerChannel1();
    void triggerChannel2();
    void triggerChannel3();
    void triggerChannel4();

    void clockSquareTimer(SquareChannelRuntime& channel, uint16_t cycles, uint16_t frequency);
    void clockWaveTimer(uint16_t cycles);
    void clockNoiseTimer(uint16_t cycles);

    void clockSquareLength(SquareChannelRuntime& channel, bool enabled);
    void clockWaveLength();
    void clockNoiseLength();

    void clockSquareEnvelope(SquareChannelRuntime& channel,uint16_t nrx2);
    void clockNoiseEnvelope();
    void clockGenericEnvelope(uint8_t& currentVolume,uint8_t& envelopeTimer,uint16_t nrx2);

    uint8_t getSquareOutput(const SquareChannelRuntime& channel, uint8_t duty) const;
    uint8_t getWaveOutput() const;
    uint8_t getNoiseOutput() const;

    StereoSample mixAudio() const;

    uint16_t readFrequency(uint16_t lowRegister,uint16_t highRegister) const;

    uint8_t readDuty(uint16_t nrx1) const;
    uint8_t readInitialVolume(uint16_t nrx2) const;
    uint8_t readEnvelopePeriod(uint16_t nrx2) const;
    bool readEnvelopeDirection(uint16_t nrx2) const;
    bool readLengthEnable(uint16_t nrx4) const;

    bool isTriggerSet(uint16_t nrx4) const;
    bool isDACEnabled(uint16_t nrx0) const;
    void updateStatusRegister();
    void checkTriggers();

    bool isDAC1Enabled() const;
    bool isDAC2Enabled() const;
    bool isDAC4Enabled() const;
};