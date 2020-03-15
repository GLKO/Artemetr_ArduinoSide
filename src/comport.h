#pragma once

#include <icomport.h>
#include <ATimer.h>

namespace Arduino {

class ComPort : public IComPort
{
public:
    ComPort();
    void sendMessage(const char *msg) override;
    const char *readMessage() override;

    void loopCheck();

private:
    ATimer _connectionTimer;
    char *message = nullptr;
};

} // namespace Arduino
