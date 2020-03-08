#pragma once

#include <icomport.h>
#include <ATimer.h>

namespace Arduino {

class ComPort : public IComPort
{
public:
    ComPort();
    void sendMessage(const char *) override;
    const char *readMessage() override;

private:
    ATimer _connectionTimer;
};

}