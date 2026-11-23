#ifndef ENGINE_CONTROLLER_HPP
#define ENGINE_CONTROLLER_HPP

#include <cstdint>
#include <vector>
#include <pigpio.h>

class EngineController {
public:
    explicit EngineController(std::vector<unsigned> pins);
    ~EngineController();

    void disarm();
    void setThrottles(const std::vector<float>& throttles);

private:
    std::vector<unsigned> pins;
    uint32_t allPinsMask = 0;
    std::vector<uint16_t> frames;
    std::vector<gpioPulse_t> pulses;

    static uint16_t throttleToDshotValue(float throttle);
    static uint16_t buildDshotFrame(uint16_t value11Bit);
    void transmitFrames();
};

#endif // ENGINE_CONTROLLER_HPP