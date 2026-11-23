#include <pigpio.h>
#include <signal.h>

#include "main.hpp"
#include "engine_controller.hpp"

void interruptHandler(int a) {
    gpioTerminate();
    exit(1);
}

int main() {
    EngineController engines({5, 6, 13, 19});

    signal(SIGINT, interruptHandler);   // for ctrl+c
    
    for (int i = 0; i < 1; ++i) {
        engines.disarm();
        gpioSleep(PI_TIME_RELATIVE, 0, 2000);
    }

    std::vector<float> testThrottles = {0.3, 0.3, 0.3, 0.3};
;
    for (int i = 0; i < 1; ++i) {
        engines.setThrottles(testThrottles);
        gpioSleep(PI_TIME_RELATIVE, 0, 1000);
    }
    
    return 0;
}
