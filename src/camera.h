#pragma once

#include <icamera.h>
#include <point.h>

class AStepper;
class IComPort;

namespace Arduino {

class Camera : public ICamera
{
public:
    Camera(AStepper* xMotor, AStepper* yMotor);
    void move(Point newPos) override;
    void moveX(int x) override;
    void moveY(int y) override;
    int currentX() const override;
    int currentY() const override;
    Point currentPos() const override;

    void update() override;

    void setComPort(IComPort *port);
    void loopCheck();

private:
    AStepper *_xMotor = nullptr,
             *_yMotor = nullptr;

    Point _currentPosition,
          _targetPosition;

    IComPort* _comPort = nullptr;
    ATimer _positionUpdateTimer;
};

}