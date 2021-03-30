#pragma once

#include "iobserver.h"

class IComPort;

class Pump : public Subscriber
{
public:
    Pump(char sign, unsigned short pin, IComPort *port);
    void publisherUpdated();

    bool isBusy() const;
    void loop();

private:
    char const _sign;
    unsigned short const _pin;
    IComPort *const _comPort;

    unsigned long _workTime = 0;
    unsigned long _afterWorkTime = 0;

    bool _busy = false;
    bool _pumpWorks = false;
    unsigned long _startTime = 0;

    void startPump();
};