#include "comport.h"
#include <Arduino.h>
#include <keywords.h>


namespace Arduino
{

ComPort::ComPort()
{
    Serial.begin(connnectionSpeed);
    _connectionTimer.setPeriod(1000);
    _connectionTimer.start();
}

void ComPort::sendMessage(const char *)
{
}

const char *ComPort::readMessage()
{
}

} // namespace Arduino