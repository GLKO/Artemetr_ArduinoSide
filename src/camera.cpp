#include "camera.h"
#include <AStepper.h>
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
    _currentPosition.X = -1;
    _currentPosition.Y = -1;
    _targetPosition.X = -1;
    _targetPosition.Y = -1;
    _positionUpdateTimer.setPeriod(200);
    _positionUpdateTimer.start();
}

void Camera::move(Point newPos)
{
    _targetPosition = newPos;
}

void Camera::moveX(int x)
{
    _targetPosition.X = x;
}

void Camera::moveY(int y)
{
    _targetPosition.Y = y;
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
    if (_positionUpdateTimer.check())
    {
        String message(currentPosition);
        message += ' ';
        message += String(_currentPosition.X);
        message += ' ';
        message += String(_currentPosition.Y);

        _comPort->sendMessage(message.c_str());
    }
    // motor control here mb
}

void Camera::update()
{
    char msg[maxMessageLength];
    strcpy(msg, _comPort->readMessage());
    char* command = msg;
    char* firstArg = strtok(msg, " ");
    char* secondArg = strtok(NULL, " ");

    if ( strcmp(command, moveTo) ) {
        Point newPos;
        newPos.X = strtol(firstArg, NULL, 10);
        newPos.Y = strtol(secondArg, NULL, 10);
        move(newPos);
    }
}

} // namespace Arduino
