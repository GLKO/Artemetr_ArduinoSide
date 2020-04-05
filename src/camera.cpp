#include "camera.h"
#include "axis.h"
#include <icomport.h>
#include <point.h>
#include <keywords.h>
#include <Arduino.h>
#include <string.h>

namespace Arduino
{

Camera::Camera(Axis *xAxis, Axis *yAxis)
    : _xAxis(xAxis),
      _yAxis(yAxis)
{
    _xAxis->setMax(450);
    _yAxis->setMax(450);
    _positionUpdateTimer.setPeriod(500);
    _positionUpdateTimer.start();
}

void Camera::setComPort(IComPort *comPort)
{
    _comPort = comPort;
}

void Camera::move(Point newPos)
{
    if ( !_isEnabled ) {
        _isEnabled = true;
        _xAxis->setEnabled(true);
        _yAxis->setEnabled(true);
    }

    _xAxis->move(newPos.X);
    _yAxis->move(newPos.Y);
    // Serial.println("New target received!");
}

void Camera::moveX(int x)
{
    _xAxis->move(x);
}

void Camera::moveY(int y)
{
    _yAxis->move(y);
}

int Camera::currentX() const
{
    return _xAxis->currentPos();
}

int Camera::currentY() const
{
    return _yAxis->currentPos();
}

Point Camera::currentPos() const
{
    Point ret(_xAxis->currentPos(), _yAxis->currentPos());
    return ret;
}

void Camera::loopCheck()
{
    // volatile auto start = micros();

    _xAxis->loopCheck();

    // volatile auto end = micros();
    // Serial.print("xAxis::loopCheck takes ");
    // Serial.println(end-start);

    _yAxis->loopCheck();

    // return;
    // start = micros();
    
    if (_positionUpdateTimer.check() && _comPort)
    {
        String message(currentPosition);
        message += ' ';
        message += String(_xAxis->currentPos());
        message += ' ';
        message += String(_yAxis->currentPos());

        _comPort->sendMessage(message.c_str());
    }
    // end = micros();
    // Serial.print("Camera::loopCheck takes ");
    // Serial.println(end-start);
}

void Camera::updateSub()
{
    const char *msg;
    msg = _comPort->readMessage();

    if ( *msg == moveTo ) {
        // Serial.println(command);
        // Serial.println(firstArg);
        // Serial.println(secondArg);
        Point newPos;
        newPos.X = *reinterpret_cast<const int *>( msg+xPos );
        newPos.Y = *reinterpret_cast<const int *>( msg+yPos );
        move(newPos);
        Serial.print(newPos.X);
        Serial.print(' ');
        Serial.println(newPos.Y);
    }
    // volatile auto end = micros();
    // Serial.print("ComPort::loopCheck takes ");
    // Serial.println(end-start);
}

} // namespace Arduino
