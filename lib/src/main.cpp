#include <pigpio.h>
#include <signal.h>
#include <stdexcept>

#include "main.hpp"
#include "engine_controller.hpp"
#include "steering.hpp"

 
void interruptHandler(int a) {
    gpioTerminate();
    exit(1);
}

int main() {
    if (gpioCfgClock(1, 1, 0) < 0) {
        throw std::runtime_error("gpioCfgClock(1us) failed");
    }
    
    if (gpioInitialise() < 0) {
        throw std::runtime_error("gpioInitialise failed. Did you use sudo?");
    }
    
    EngineController engines({5, 23, 20, 27});
    Steering steering(engines);

    signal(SIGINT, interruptHandler);   // for ctrl+c
    
    for (int i = 0; i < 1000; ++i) {
        engines.disarm();
        gpioSleep(PI_TIME_RELATIVE, 0, 2000);
    }

    steering.listen();

    return 0;
}
