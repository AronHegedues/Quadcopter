#ifndef ENGINE_H
#define ENGINE_H

class Engine {
public:
    Engine(int escPin, int minPulseThrottle, int maxPulseThrottle);
    ~Engine();

    void arm();
    void disarm();
    void throttle(float percent);
    void throttle(float percent, int seconds, int microseconds);

private:
    int pin;
    int minPulse;
    int maxPulse;
};

#endif