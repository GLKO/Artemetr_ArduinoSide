#include "axis.h"
#include <math.h>
#include <Arduino.h>

Axis::Axis(uint32_t acceleration, uint32_t startSpeed)
{
    _currentSpeed = (_currentSpeed + sqrt(_currentSpeed * _currentSpeed + 4 * _acceleration)) / 2;
    // p = 1000000 / V;
}

void Axis::moveTo(uint32_t newPos)
{

}

void Axis::loopCheck()
{
    if ( _currentPos == _targetPos )
        return;

    if ( !timeCheck() )
        return;
    reversCheck();
    accelerationCheck();
    step();
    

    if (_currentPos < _targetPos) {

    } else {

    }
}

bool Axis::timeCheck()
{
    auto currentTime = micros();
    if (currentTime - _lastTime < _period)
        return false;

    _lastTime = currentTime;
    return true;
}

void Axis::reversCheck()
{
    if (_currentSpeed > _startSpeed)
        return;

    if ( (!_revers) && (_targetPos > _currentPos) )
        return;
    else if ( _revers && (_targetPos < _currentPos) )
        return;

    _revers = !_revers;
    digitalWrite(_pinDirection, _revers); //WARNING

    _startBreakingPos = (_targetPos - _currentPos)/2;
}

void Axis::accelerationCheck()
{

}

void Axis::step()
{
    digitalWrite(_pinPulse, HIGH);
    digitalWrite(_pinPulse, LOW);
    
    if (_revers)
        --_currentPos;
    else
        ++_currentPos;
}