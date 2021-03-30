#pragma once

#include <icomport.h>
#include <UniversalTimer.h>
#include <keywords.h>

namespace Arduino {

class ComPort : public IComPort
{
public:
    ComPort();
    void init();

    void sendMessage(const char *msg) override;
    const char *readMessage() override;

    void loopCheck();

private:
    bool _connected = false;
    UniversalTimer _connectionTimer;
    char message[commandSize] = {0};
};

} // namespace Arduino
