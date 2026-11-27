#ifndef STEERING_H
#define STEERING_H

#include <cstdint>
#include <vector>
#include <pigpio.h>
#include <termios.h>

#include "engine_controller.hpp"

class Steering {
public:
    explicit Steering(EngineController &engine);

    void listen();

private:
    EngineController engines;
    termios newt, oldt;
};

#endif 