#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <stdlib.h>

#include "main.h"
#include "controls.h"

int main() {
    if (gpioInitialise() < 0) { // requires sudo
        fprintf(stderr, "Failed to initialize pigpio. Did you run with sudo?\n");
        return EXIT_FAILURE;
    }
    printf("Startup successfull.\n");

    gpioSetMode(ESC_GPIO, PI_OUTPUT);
    gpioServo(ESC_GPIO, NEUTRAL_PULSE);
    printf("Sending neutral signal to GPIO %d to arm ESC...\n", ESC_GPIO);

    gpioSleep(PI_TIME_RELATIVE, 1, 0);
    for (int i = 0; i <= 50; i++) {
        throttle(i);
        gpioSleep(PI_TIME_RELATIVE, 0, 50000);
    }

    gpioSleep(PI_TIME_RELATIVE, 3, 0);
    
    
    gpioServo(ESC_GPIO, NEUTRAL_PULSE);
    printf("Sending neutral signal to GPIO %d to disarm ESC...\n", ESC_GPIO);

    gpioSleep(PI_TIME_RELATIVE, 1, 0);

    gpioServo(ESC_GPIO, 0);
    gpioTerminate();   
    
    printf("Exit sucessfull\n");
    return 0;
}