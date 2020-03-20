#include "comport.h"
#include <Arduino.h>

namespace Arduino
{

ComPort::ComPort()
{
    _connectionTimer.setPeriod(2000);
    _connectionTimer.start();
}

void ComPort::init()
{
    Serial.end();
    Serial.begin(connnectionSpeed);
    Serial.setTimeout(5);
}

void ComPort::sendMessage(const char *msg)
{
    if ( _connected)
        Serial.println(msg);
}

const char *ComPort::readMessage()
{
    return message;
}

void ComPort::loopCheck()
{
    if ( _connected ) {
        if ( Serial.available() == 0)
            return;

        String msg = Serial.readString();
        // Serial.println(msg);

        strcpy(message, msg.c_str());  //если сильно хочется, то можно не копировать
        notifySubscribers();
    } 
    else {
        if (_connectionTimer.check() ) {
            Serial.println(movingCamId);

            if (Serial.find(connectRequest)){
                // Serial.readString();
                Serial.println(connectApprove);
                _connected = true;
                _connectionTimer.stop();
            }
        }
    }
}

} // namespace Arduino