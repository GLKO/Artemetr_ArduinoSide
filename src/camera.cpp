#include "camera.h"
#include <AStepper.h>
#include <ATimer.h>
#include <icomport.h>
#include <keywords.h>
#include <Arduino.h>
#include <string.h>

namespace Arduino
{

Camera::Camera(AStepper *xMotor, AStepper *yMotor)
    : _xMotor(xMotor),
      _yMotor(yMotor)
{
    _positionUpdateTimer.setPeriod(200);
    _positionUpdateTimer.start();
}

void Camera::move(Point newPos)
{
}

void Camera::moveX(int x)
{
}

void Camera::moveY(int y)
{
}

int Camera::currentX() const
{
    return _currentPosition.X;
}

int Camera::currentY() const
{
    return _currentPosition.Y;
}

Point Camera::currentPos() const
{
    return _currentPosition;
}

void Camera::loopCheck()
{
    if ( _positionUpdateTimer.check() )
    {
        String message(currentPosition);
        message += ' ';
        message += String(rand());
        message += ' ';
        message += String(rand());

        // Serial.println(message);
        _comPort->sendMessage(message.c_str());
    }
}

void Camera::update()
{
    String message( _comPort->readMessage() );
    // strtok(message.c_str(), ' ');
}

} // namespace Arduino