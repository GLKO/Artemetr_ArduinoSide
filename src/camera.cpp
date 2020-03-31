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
    _positionUpdateTimer.setPeriod(100);
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
    _xAxis->loopCheck();
    _yAxis->loopCheck();

    if (_positionUpdateTimer.check())
    {
        String message(currentPosition);
        message += ' ';
        message += String(_xAxis->currentPos());
        message += ' ';
        message += String(_yAxis->currentPos());

        if ( _comPort != nullptr )
            _comPort->sendMessage(message.c_str());
    }
}

void Camera::updateSub()
{
    char msg[maxMessageLength];
    strcpy(msg, _comPort->readMessage());
    char* command = strtok(msg, " ");
    char* firstArg = strtok(NULL, " ");
    char* secondArg = strtok(NULL, " ");

    if ( strcmp(command, moveTo) == 0 ) {
        Point newPos;
        newPos.X = strtol(firstArg, NULL, 10);
        newPos.Y = strtol(secondArg, NULL, 10);
        move(newPos);
    }
}

} // namespace Arduino
