#include <pigpio.h>
#include <stdio.h>

#include "controls.h"
#include "main.h"

void throttle(float percent) {
    percent /= 100;
    int range = MAX_PULSE_THROTTLE - MIN_PULSE_THROTTLE;

    gpioServo(ESC_GPIO, MIN_PULSE_THROTTLE + (int) (range * percent));
}