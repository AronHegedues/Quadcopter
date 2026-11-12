#include <pigpio.h>
#include <stdio.h>

#include "controls.hpp"
#include "main.hpp"

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
    printf("Armed ESC at pin %i\n", pin);
}

void Engine::disarm() {
    gpioServo(pin, 1000);    // 1000 = neutral pulse
    printf("Disarmed ESC at pin %i\n", pin);
}

void Engine::throttle(float percent) {
    percent /= 100;
    int range = maxPulse - minPulse;
    
    gpioServo(pin, minPulse + (int) (range * percent));
}
