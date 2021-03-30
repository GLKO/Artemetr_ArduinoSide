#include "pump.h"

#include "icomport.h"
#include "keywords.h"
#include <Arduino.h>


Pump::Pump(char sign, unsigned short pin, IComPort *port)
    : _sign(sign),
      _pin(pin),
      _comPort(port)
{
    pinMode(_pin, OUTPUT);
}

void Pump::publisherUpdated()
{
    const char *msg;
    msg = _comPort->readMessage();
    if ( *msg != _sign ) return;

    _workTime = *reinterpret_cast<const int *>( msg+xPosIndex );
    _afterWorkTime = *reinterpret_cast<const int *>( msg+yPosIndex );

    startPump();
}

void Pump::startPump()
{
    _busy = true;
    digitalWrite(_pin, HIGH);
    _pumpWorks = true;
    _startTime = millis();
}

bool Pump::isBusy() const
{
    return _busy;
}

void Pump::loop()
{
    if ( !_busy ) return;

    auto currentTime = millis();
    if ( currentTime - _startTime > _workTime + _afterWorkTime ) _busy = false;
    if ( _pumpWorks && currentTime - _startTime > _workTime ) 
    {
        digitalWrite(_pin, LOW);
        _pumpWorks = false;
    }
}
