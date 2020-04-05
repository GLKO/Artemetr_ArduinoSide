#pragma once
#include <inttypes.h>

class Axis 
{
public:
    Axis(uint32_t acceleration, uint32_t startSpeed, short pinPulse, short pinDirection, short pinEnabled, short stepsPerMm);
    // need home feature, endstops
    void setMax(unsigned mmMaxPos);
    void move(uint32_t newPos);
    void setEnabled(bool enabled);

    int currentPos();
    void loopCheck();

private:
    // HARDWARE PARAMETERS //
    short _pinPulse = 0,
          _pinDirection = 0,
          _pinEnabled = 0,
          _stepsPerMm = 0;
    unsigned long _maxPos = 0;
    ////////////////////////
    bool _isEnabled = false;
    const unsigned long _acceleration = 0;
    double _startSpeed = 0;
    double _currentSpeed = 0;

                
             
    bool timeCheck();
    unsigned long _lastTime = 0;

    void reversCheck();
    bool _revers = false;
    bool rightDirection();

    void step();
    unsigned long _targetPos = 0; 
    unsigned long _currentPos = 0;

    void accelerationCheck();
    bool _accelerate = true;
    void calculateNewSpeed();
    unsigned long _period = 0;

    void home();
};