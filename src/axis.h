#pragma once
#include <inttypes.h>

enum EndstopPosition { EndstopAtMin, EndstopAtMax };

class Axis 
{
public:
    Axis(short stepsPerMm, uint8_t pinPulse, uint8_t pinDirection, uint8_t pinEnabled, uint8_t pinEndstop, EndstopPosition endstopPos);
    void setMaxPos(unsigned mmMaxPos);
    void setSpeed(int acceleration, int startSpeed, int homeSpeed);
    void invertDirection();
    
    void move(uint32_t newPos);
    void setEnabled(bool enabled);

    int currentPos();
    void loopCheck();

private:
    unsigned long _acceleration = 0;
    double _startSpeed = 0;
    double _currentSpeed = 0;
             
    bool timeCheck();
    unsigned long _lastTime = 0;

    void reversCheck();
    void setRevers(const bool &revers);
    bool _revers = false;
    bool rightDirection();
    bool _directionInverted = false;

    void step();
    unsigned long _targetPos = 0; 
    unsigned long _currentPos = 0;

    void accelerationCheck();
    bool _accelerate = true;
    void calculateNewSpeed();
    unsigned long _period = 0;

    void enable();
    void homeStep();
    void disable();
    bool _isEnabled = false;
    unsigned long _homeSpeed = 0;


        // HARDWARE PARAMETERS //
    short _pinPulse = 0,
          _pinDirection = 0,
          _pinEnabled = 0,
          _pinEndstop = 0,
          _stepsPerMm = 0;
    EndstopPosition _endstopPosition = EndstopAtMin;
    unsigned long _maxPos = 0;
    ////////////////////////

    // static bool periodsReady;
    // static unsigned period[400];
};