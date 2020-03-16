#include "axis.h"
#include <math.h>
#include <Arduino.h>

Axis::Axis(uint32_t acceleration, uint32_t startSpeed)
{
    home();
}

void Axis::moveTo(uint32_t newPos)
{
    _targetPos = newPos;
}

void Axis::loopCheck()
{
    if ( _currentPos == _targetPos )
        return;

    if ( !timeCheck() )
        return;
        
    reversCheck();
    step();

    accelerationCheck();
    calculateNewSpeed();
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

    if ( rightDirection() )
        return;

    _revers = !_revers;
    digitalWrite(_pinDirection, _revers); //WARNING! may be revers plug
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

bool Axis::rightDirection()
{
    if ( (!_revers) && (_targetPos > _currentPos) )
        return true;
    else if ( _revers && (_targetPos < _currentPos) )
        return true;
    
    return false;
}

void Axis::accelerationCheck()
{
    if ( !rightDirection() && _accelerate){
        _accelerate = false;
        return;
    }

    uint32_t leftDistanse = abs( _targetPos - _currentPos );
    uint32_t breakingDistance = _currentSpeed*_currentSpeed/_acceleration/2;

    if ( leftDistanse > breakingDistance )
        _accelerate = true;
    else
        _accelerate = false;
}

void Axis::calculateNewSpeed()
{
    if ( _accelerate )
        _currentSpeed = sqrt( _currentSpeed*_currentSpeed + 2*_acceleration );
    else
        _currentSpeed = sqrt( _currentSpeed*_currentSpeed - 2*_acceleration );

    _period = 1000000/_currentSpeed;
}

void Axis::home()
{

}