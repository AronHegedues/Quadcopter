#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

#include "steering.hpp"

Steering::Steering(EngineController &engines)
        : engines(engines) {
    termios oldt{}, newt{};

    // ---------------------------------
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable line buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    // ---------------------------------
}

void Steering::listen() {
    char ch = 0;
    std::vector<float> throttles = {.0f, .0f, .0f, .0f};
    while (true) {
        std::vector<bool> pressed = {false, false, false, false};
        while (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == 'q' || ch == 'Q') pressed[0] = true;
            if (ch == 'w' || ch == 'W') pressed[1] = true;
            if (ch == 'e' || ch == 'E') pressed[2] = true;
            if (ch == 'r' || ch == 'R') pressed[3] = true;
            else if (ch == 't') goto done; // Press 'q' to quit cleanly
        }

        if (pressed[0]) {
            std::cout << "Q received\n";
            throttles[0] += 0.025;
        }
        else {
            throttles[0] -= 0.1;
        }
        if (pressed[1]) {
            std::cout << "W received\n";
            throttles[1] += 0.025;
        }
        else {
            throttles[1] -= 0.1;
        }
        if (pressed[2]) {
            std::cout << "E received\n";
            throttles[2] += 0.025;
        }
        else {
            throttles[2] -= 0.1;
        }
        if (pressed[3]) {
            std::cout << "R received\n";
            throttles[3] += 0.025;
        }
        else {
            throttles[3] -= 0.1;
        }

        for (auto& x : throttles) {
            if (x > 1) x = 1;
            if (x < 0) x = 0;
            
        }

        for (auto& x : throttles) {
            std::cout << x << " ";   
        }
        
        std::cout << '\n';

        engines.setThrottles(throttles);
        usleep(50000);
    }

done:
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings
}
