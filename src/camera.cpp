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
    _xAxis->setMaxPos(450);
    _yAxis->setMaxPos(450);
    _positionUpdateTimer.setPeriod(500);
    _positionUpdateTimer.start();
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
    volatile unsigned long start, end;
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
        *message = currentPosition;
        int *x = reinterpret_cast<int *>(message + xPos);
        int *y = reinterpret_cast<int *>(message + yPos);
        *x = static_cast<int16_t>(_xAxis->currentPos());
        *y = static_cast<int16_t>(_yAxis->currentPos());
        // String message(currentPosition);
        // message += ' ';
        // message += String(_xAxis->currentPos());
        // message += ' ';
        // message += String(_yAxis->currentPos());

        _comPort->sendMessage(message);
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
