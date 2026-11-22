#include <pigpio.h>
#include <iostream>

#include "engine.hpp"
#include "main.hpp"

using namespace std;

Engine::Engine(int escPin, int minPulseThrottle, int maxPulseThrottle)
    : pin(escPin),
        minPulse(minPulseThrottle),
        maxPulse(maxPulseThrottle) {}

Engine::~Engine() {
    gpioServo(pin, 0);
}

void Engine::arm() {
    gpioSetMode(pin, PI_OUTPUT);
    gpioServo(pin, 1000);    // 1000 = neutral pulse
    gpioSleep(PI_TIME_RELATIVE, 2, 0);
    printf("Armed ESC at pin %i\n", pin);
}

void Engine::disarm() {
    gpioServo(pin, 1000);    // 1000 = neutral pulse
    printf("Disarmed ESC at pin %i\n", pin);
}

void Engine::throttle(float percent) {
    percent /= 100;
    int range = maxPulse - minPulse;

    cout << minPulse + (int) (range * percent) << "\n";
    gpioServo(pin, minPulse + (int) (range * percent));
}

void Engine::throttle(float percent, int seconds, int microseconds) {
    Engine::throttle(percent);
    gpioSleep(PI_TIME_RELATIVE, seconds, microseconds);
}
