#pragma once
#include <inttypes.h>

class Axis 
{
public:
    Axis(uint32_t acceleration, uint32_t startSpeed, short stepsPerMm);
    // need home feature, endstops and maxPos
    void move(uint32_t newPos);
    int currentPos();
    void loopCheck();

private:
    // HARDWARE PARAMETERS
    short _pinPulse = 0,
          _pinDirection = 0,
          _pinSleep = 0,
          _stepsPerMm = 0;
          
    const unsigned long _acceleration = 0,
                        _startSpeed = 10;

    unsigned long _targetPos = 0;
    unsigned long long _currentSpeed = 0;             
             
    bool timeCheck();
    unsigned long _lastTime = 0;

    void reversCheck();
    bool _revers = false;
    bool rightDirection();

    void step();
    unsigned long _currentPos = 0;

    void accelerationCheck();
    bool _accelerate = true;
    void calculateNewSpeed();
    unsigned long _period = 0;

    void home();
};