#include "axis.h"
#include <math.h>
#include <Arduino.h>

Axis::Axis(uint32_t acceleration, uint32_t startSpeed, short stepsPerMm)
    :_stepsPerMm(stepsPerMm),
    _acceleration(acceleration * _stepsPerMm),
    _startSpeed(startSpeed * _stepsPerMm)
{
    home();
}

void Axis::move(uint32_t newPos)
{
    _targetPos = newPos*_stepsPerMm;
}

int Axis::currentPos()
{
    return _currentPos / _stepsPerMm;
}

void Axis::loopCheck()
{
    if ( _currentPos == _targetPos ) {
        if ( _currentSpeed != _startSpeed )
            _currentSpeed = _startSpeed;
        return;
    }

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
    // digitalWrite(_pinDirection, _revers); //WARNING! may be revers plug
}

void Axis::step()
{
    // digitalWrite(_pinPulse, HIGH);
    // digitalWrite(_pinPulse, LOW);
    
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
    if ( !rightDirection() ){
        _accelerate = false;
        return;
    }

    unsigned long leftDistanse = 0;
    if ( _currentPos > _targetPos )
        leftDistanse = _currentPos - _targetPos;
    else
        leftDistanse = _targetPos - _currentPos;

    auto breakingDistance = _currentSpeed*_currentSpeed/(_acceleration*2);

    if ( leftDistanse <= breakingDistance )
        _accelerate = false;
    else
        _accelerate = true;
}

void Axis::calculateNewSpeed()
{
    if ( _accelerate )
        _currentSpeed = sqrt( _currentSpeed*_currentSpeed + 2*_acceleration );
    else
        _currentSpeed = sqrt( _currentSpeed*_currentSpeed - 2*_acceleration );
    // Serial.print( static_cast<unsigned long>(_currentSpeed) );
    _period = 1000000/_currentSpeed;
}

void Axis::home()
{

}