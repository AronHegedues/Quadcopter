#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.hpp"
#include "engine.hpp"

void interruptHandler(int a) {
    gpioTerminate();
}

int main() {
    if (gpioInitialise() < 0) {
        printf("Failed to initialise gpio. Did you use sudo?\n");
        return -1;
    }
    constexpr int MIN_PULSE_THROTTLE = 1020;
    constexpr int MAX_PULSE_THROTTLE = 2000;

    signal(SIGINT, interruptHandler);   // for ctrl+c
    
    Engine engine1(19, MIN_PULSE_THROTTLE, MAX_PULSE_THROTTLE);

    engine1.arm();

    engine1.throttle(3.0f, 1, 0);

    engine1.disarm();
    gpioTerminate();

    printf("Exit successful\n");
    return 0;
}
