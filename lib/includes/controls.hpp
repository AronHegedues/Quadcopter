#ifndef CONTROLS_H
#define CONTROLS_H

class Engine {
public:
    Engine(int escPin, int minPulseThrottle, int maxPulseThrottle);
    ~Engine();

    void arm();
    void disarm();
    void throttle(float percent);

private:
    int pin;
    int minPulse;
    int maxPulse;
};

#endif