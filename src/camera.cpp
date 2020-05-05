#include "camera.h"
#include "axis.h"
#include <icomport.h>
#include <point.h>
#include <keywords.h>
#include <Arduino.h>
#include <string.h>

namespace Arduino
{

Camera::Camera(Axis *xAxis, Axis *yAxis, uint8_t pinBacklight)
    : _xAxis(xAxis),
      _yAxis(yAxis),
      _pinBacklight(pinBacklight)
{
    _positionUpdateTimer.setPeriod(200);
    _positionUpdateTimer.start();

    pinMode(pinBacklight, OUTPUT);
    setBacklight(Off);
}

void Camera::setComPort(IComPort *comPort)
{
    _comPort = comPort;
}

void Camera::move(Point newPos)
{
    _xAxis->move(newPos.X);
    _yAxis->move(newPos.Y);
    // Serial.println("New target received!");
}

void Camera::setBacklight(BacklightState state)
{
    if ( state == On ) {
        digitalWrite(_pinBacklight, HIGH);
    } else 
    if ( state == Off ){
        digitalWrite(_pinBacklight, LOW);
    }
}

Point Camera::currentPos() const
{
    Point ret(_xAxis->currentPos(), _yAxis->currentPos());
    return ret;
}

Point Camera::targetPos() const
{
    return Point();
}

void Camera::loopCheck()
{
    // volatile unsigned long start, end;
    // start = micros();

    _xAxis->loopCheck();

    // end = micros();
    // Serial.print("xAxis::loopCheck takes ");
    // Serial.println(end-start);

    _yAxis->loopCheck();

    // return;
    // start = micros();
    
    if (_positionUpdateTimer.check() && _comPort)
    {
        char message[commandSize] = {0};
        *message = currentCamPosition;
        int *x = reinterpret_cast<int *>(message + xPosIndex);
        int *y = reinterpret_cast<int *>(message + yPosIndex);
        *x = static_cast<int16_t>(_xAxis->currentPos());
        *y = static_cast<int16_t>(_yAxis->currentPos());

        _comPort->sendMessage(message);
    }
    // end = micros();
    // Serial.print("Camera::loopCheck takes ");
    // Serial.println(end-start);
}

void Camera::publisherUpdated()
{
    const char *msg;
    msg = _comPort->readMessage();

    if ( *msg == moveCamTo ) {
        Point newPos;
        newPos.X = *reinterpret_cast<const int *>( msg+xPosIndex );
        newPos.Y = *reinterpret_cast<const int *>( msg+yPosIndex );
        move(newPos);
    } else
    if ( *msg == setBacklightState ) {
        BacklightState state = *reinterpret_cast<const BacklightState*>( msg+backlightStateIndex );
        setBacklight(state);
    }
    // volatile auto end = micros();
    // Serial.print("ComPort::loopCheck takes ");
    // Serial.println(end-start);
}

} // namespace Arduino
