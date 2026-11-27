#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <pigpio.h>

#include "engine_controller.hpp"
#include "main.hpp"

using namespace std;

EngineController::EngineController(vector<unsigned> pins)
    : pins(move(pins)) {
    if (this->pins.empty()) {
        throw invalid_argument("No pins given");
    }

    for (unsigned pin : this->pins) {
        if (pin > 31) {
            throw invalid_argument("GPIO pin must be between 0 and 31");
        }
        allPinsMask |= (1U << pin);
    }

    for (unsigned pin : this->pins) {
        gpioSetMode(pin, PI_OUTPUT);
        gpioWrite(pin, 0);
    }

    // 16 Bits * 3 = 48 pulses (independent of motor count)
    pulses.reserve(48);
    frames.resize(this->pins.size(), 0);
}

EngineController::~EngineController() {
    disarm();
    gpioTerminate();
}

void EngineController::disarm() {
    uint16_t stopFrame = buildDshotFrame(0);
    fill(frames.begin(), frames.end(), stopFrame);
    transmitFrames();
}

void EngineController::setThrottles(const vector<float>& throttles) {
    for (size_t i = 0; i < throttles.size(); ++i) {
        uint16_t value = throttleToDshotValue(throttles[i]);
        frames[i] = buildDshotFrame(value);
    }
    transmitFrames();
}

uint16_t EngineController::throttleToDshotValue(float throttle) {
    if (throttle <= 0.0001f) {
        return 0;
    }
    if (throttle >= 1.0f) {
        return 2047;
    }

    // Values lower than 48 are reserved for special commands
    return static_cast<uint16_t>(48.0f + throttle * (2047.0f - 48.0f));
}

uint16_t EngineController::buildDshotFrame(uint16_t value11Bit) {
    uint16_t packet = (value11Bit & 0x07FF) << 1; // << 1 because telemetry bit is 0
    uint8_t crc = (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x0F;
    return static_cast<uint16_t>((packet << 4) | crc);
}

void EngineController::transmitFrames() {
    pulses.clear();

    for (int bit = 15; bit >= 0; --bit) {
        uint32_t zeroMask = 0;
        uint32_t oneMask = 0;

        for (size_t m = 0; m < pins.size(); ++m) {
            bool isOne = (frames[m] >> bit) & 1;
            if (isOne) {
                oneMask |= (1 << pins[m]);
            } else {
                zeroMask |= (1 << pins[m]);
            }
        }

        pulses.push_back({allPinsMask, 0, 2});
        pulses.push_back({0, zeroMask, 3});
        pulses.push_back({0, oneMask, 2});
    }

    gpioWaveAddNew();
    gpioWaveAddGeneric(static_cast<unsigned>(pulses.size()), pulses.data());
    int waveId = gpioWaveCreate();

    if (waveId >= 0) {
        gpioWaveTxSend(waveId, PI_WAVE_MODE_ONE_SHOT);
        while (gpioWaveTxBusy()) {
            gpioSleep(PI_TIME_RELATIVE, 0, 5);
        }
        gpioWaveDelete(waveId);
    }
}