#pragma once

#include <icomport.h>
#include <ATimer.h>
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
    ATimer _connectionTimer;
    char message[maxMessageLength] = {0};
};

} // namespace Arduino
