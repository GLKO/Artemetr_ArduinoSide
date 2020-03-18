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
    const uint32_t _acceleration = 0,
                   _startSpeed = 10;

    uint32_t _targetPos = 0,
             _currentSpeed = 0;             
             
    bool timeCheck();
    uint32_t _lastTime = 0;

    void reversCheck();
    bool _revers = false;
    bool rightDirection();

    void step();
    uint32_t _currentPos = 0;

    void accelerationCheck();
    bool _accelerate = true;
    void calculateNewSpeed();
    uint32_t _period = 0;

    void home();
    
    // HARDWARE PARAMETERS
    short _pinPulse = 0,
          _pinDirection = 0,
          _pinSleep = 0,
          _stepsPerMm = 0;
};