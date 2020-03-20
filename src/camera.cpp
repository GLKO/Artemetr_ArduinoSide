#include "camera.h"
#include "axis.h"
#include <icomport.h>
#include <keywords.h>
#include <Arduino.h>
#include <string.h>

namespace Arduino
{

Camera::Camera(Axis *xAxis, Axis *yAxis)
    : _xAxis(xAxis),
      _yAxis(yAxis)
{
    // _currentPosition.X = -1;
    // _currentPosition.Y = -1;
    // _targetPosition.X = -1;
    // _targetPosition.Y = -1;
    _positionUpdateTimer.setPeriod(1000);
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
    // _targetPosition = newPos;
}

void Camera::moveX(int x)
{
    // _targetPosition.X = x;
}

void Camera::moveY(int y)
{
    // _targetPosition.Y = y;
}

int Camera::currentX() const
{
    // return _currentPosition.X;
}

int Camera::currentY() const
{
    // return _currentPosition.Y;
}

Point Camera::currentPos() const
{
    // return _currentPosition;
}

void Camera::loopCheck()
{
    _xAxis->loopCheck();
    _yAxis->loopCheck();

    if (_positionUpdateTimer.check())
    {
        String message(currentPosition);
        message += ' ';
        message += String(_xAxis->currentPos());
        message += ' ';
        message += String(_yAxis->currentPos());

        // if ( _comPort != nullptr )
            // _comPort->sendMessage(message.c_str());
    }
}

void Camera::updateSub()
{
    char msg[maxMessageLength];
    strcpy(msg, _comPort->readMessage());
    char* command = strtok(msg, " ");
    char* firstArg = strtok(NULL, " ");
    char* secondArg = strtok(NULL, " ");

    Serial.print(command);
    Serial.print(" ");
    Serial.print(firstArg);
    Serial.print(" ");
    Serial.println(secondArg);

    if ( strcmp(command, moveTo) ) {
        Point newPos;
        newPos.X = strtol(firstArg, NULL, 10);
        newPos.Y = strtol(secondArg, NULL, 10);
        move(newPos);
    }
}

} // namespace Arduino
