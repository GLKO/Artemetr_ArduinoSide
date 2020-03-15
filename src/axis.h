#pragma once
#include <inttypes.h>

class Axis 
{
public:
    Axis(uint32_t acceleration, uint32_t startSpeed);

    void moveTo(uint32_t newPos);
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

    void accelerationCheck();
    bool acceleration = true;
    uint32_t _period = 0,
             _startBreakingPos = 0;

    void step();
    uint32_t _currentPos = 0;

    // HARDWARE PARAMETERS
    short _pinPulse = 0,
          _pinDirection = 0,
          _pinSleep = 0,
          _sepsPerMm = 0;
};