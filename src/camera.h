#pragma once

#include <icamera.h>
#include <UniversalTimer.h>
#include <inttypes.h>

struct Point;
class Axis;
class IComPort;

namespace Arduino
{

class Camera : public ICamera
{
public:
    Camera(Axis *xAxis, Axis *yAxis, uint8_t pinBacklight);
    void move(Point newPos) override;
    Point currentPos() const override;
    Point targetPos() const override;
    void setBacklight(BacklightState state) override;

    void publisherUpdated() override;

    void setComPort(IComPort *port);
    void loopCheck();

private:
    Axis *_xAxis = nullptr,
         *_yAxis = nullptr;
    short _pinBacklight = 0;

    IComPort *_comPort = nullptr;
    UniversalTimer _positionUpdateTimer;
};

} // namespace Arduino
