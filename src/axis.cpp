#include "axis.h"
#include <math.h>
#include <Arduino.h>

Axis::Axis( short stepsPerMm, uint8_t pinPulse, uint8_t pinDirection, uint8_t pinEnabled, uint8_t pinEndstop, EndstopPosition endstopPos )
    :_pinPulse(pinPulse),
    _pinDirection(pinDirection),
    _pinEnabled(pinEnabled),
    _pinEndstop(pinEndstop),
    _stepsPerMm(stepsPerMm),
    _endstopPosition(endstopPos)
{
    pinMode( _pinPulse, OUTPUT );
    pinMode( _pinDirection, OUTPUT );
    pinMode( _pinEnabled, OUTPUT );
    pinMode( _pinEndstop, INPUT_PULLUP);

    disable();
}

void Axis::setMaxPos(unsigned mmMaxPos)
{
    _maxPos = mmMaxPos * _stepsPerMm;
}

void Axis::setSpeed( int acceleration, int startSpeed, int homeSpeed )
{
    _acceleration = acceleration * _stepsPerMm;

    if ( startSpeed == 0 )
        _startSpeed = 3;
    else
        _startSpeed = startSpeed * _stepsPerMm;

    _homeSpeed = homeSpeed * _stepsPerMm;
}

void Axis::invertDirection()
{
    _directionInverted = !_directionInverted;
}

void Axis::setEnabled(bool setOn)
{
    if ( setOn && !_isEnabled) {
        enable();
    }
    else if ( !setOn && _isEnabled ) {
        disable();
    }
}

void Axis::move(uint32_t newPos)
{
    if ( !_isEnabled )
        enable();

    _targetPos = newPos*_stepsPerMm;

    if (_targetPos > _maxPos)
        _targetPos = _maxPos;
}

int Axis::currentPos()
{
    if ( _isEnabled )
        return _currentPos / _stepsPerMm;
    else
        return -1;
}

void Axis::loopCheck()
{
    if ( !_isEnabled ) {
        homeStep();
        return;
    }

    if ( _currentPos == _targetPos  ) { //&& _currentSpeed < _startSpeed
        _currentSpeed = _startSpeed;
        return;
    }

    if ( timeCheck() ) {
        reversCheck();
        step();

        accelerationCheck();
        calculateNewSpeed();
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

    if ( !rightDirection() )
        _revers = !_revers;

    setRevers(_revers);
}

void Axis::setRevers(const bool &revers)
{
    if ( _directionInverted )
        digitalWrite(_pinDirection, !revers);
    else
        digitalWrite(_pinDirection, revers);
}

void Axis::step()
{
    digitalWrite(_pinPulse, HIGH);
    digitalWrite(_pinPulse, LOW);
    
    if (_revers ) {
        if ( _currentPos > 0 )
            --_currentPos;
    }
    else {
        ++_currentPos;
    }
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

    _period = 1000000/_currentSpeed;
}

void Axis::enable()
{
    digitalWrite( _pinEnabled, LOW);
    if ( digitalRead(_pinEndstop) == HIGH ) {
        _period = 1000000/_homeSpeed;
        if ( _endstopPosition == EndstopAtMin )
            setRevers(true);
        else
            setRevers(false);
    }
}

void Axis::homeStep()
{
    //mb check that pin enabled
    if ( timeCheck() ) {
        if ( digitalRead( _pinEndstop ) == HIGH) {
            digitalWrite(_pinPulse, HIGH);
            digitalWrite(_pinPulse, LOW);
        } else {
            _isEnabled = true;
            if ( _endstopPosition == EndstopAtMin )
                _currentPos = 0;
            else
                _currentPos = _maxPos;
        }
    }
}

void Axis::disable() 
{
    digitalWrite( _pinEnabled, HIGH);
    _isEnabled = false;
}
