#include "comport.h"
#include <Arduino.h>

namespace Arduino
{

ComPort::ComPort()
{
    Serial.begin(connnectionSpeed);
    _connectionTimer.setPeriod(1000);
    _connectionTimer.start();
}

void ComPort::sendMessage(const char *msg)
{
    Serial.println(msg);
}

const char *ComPort::readMessage()
{
    return message;
}

void ComPort::loopCheck()
{
    static bool connected = false;

    if ( connected ) {
        if ( Serial.available < 4)
            return;

        String msg = Serial.readString;

        if (msg.length() < maxMessageLength) {
            strcpy(message, msg.c_str);  //если сильно хочется, то можно не копировать
            notifySubscribers();
        }
    } else {
        if (_connectionTimer.check() ) {
            Serial.println(movingCamId);

            if (Serial.find(connectRequest)){
                Serial.println(connectApprove);
                connected = true;
                _connectionTimer.stop();
            }
        }
    }
}

} // namespace Arduino