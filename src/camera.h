#pragma once

#include <icamera.h>
#include <point.h>
#include <ATimer.h>

class Axis;
class IComPort;

namespace Arduino
{

class Camera : public ICamera
{
public:
    Camera(Axis *xAxis, Axis *yAxis);
    void move(Point newPos) override;
    void moveX(int x) override;
    void moveY(int y) override;
    int currentX() const override;
    int currentY() const override;
    Point currentPos() const override;

    void updateSub() override;

    void setComPort(IComPort *port);
    void loopCheck();

private:
    Axis *_xAxis = nullptr,
         *_yAxis = nullptr;

    // Point _currentPosition,
        //   _targetPosition;

    IComPort *_comPort = nullptr;
    ATimer _positionUpdateTimer;
};

} // namespace Arduino
