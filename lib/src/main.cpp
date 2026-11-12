#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdlib.h>

#include "main.hpp"
#include "controls.hpp"


int main() {
    if (gpioInitialise() < 0) {
        printf("Sudo?\n");
        return -1;
    }
    constexpr int MIN_PULSE_THROTTLE = 1109;
    constexpr int MAX_PULSE_THROTTLE = 1900;

    Engine engine1(19, MIN_PULSE_THROTTLE, MAX_PULSE_THROTTLE);

    engine1.arm();

    gpioSleep(PI_TIME_RELATIVE, 3, 0);
    engine1.throttle(30.0f);

    gpioSleep(PI_TIME_RELATIVE, 3, 0);

    engine1.disarm();
    gpioTerminate();

    printf("Exit successful\n");
    return 0;
}