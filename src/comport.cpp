#include "comport.h"
#include <Arduino.h>

namespace Arduino
{

ComPort::ComPort()
{
    _connectionTimer.setPeriod(1000);
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

        if (Serial.find(disconnectRequest)){
            _connected = false;
            _connectionTimer.start();
            return;
        }

        String msg = Serial.readString();
        strcpy(message, msg.c_str());
        notifySubscribers();
    } 
    else {
        if (_connectionTimer.check() ) {
            Serial.println(movingCamId);

            if (Serial.find(connectRequest)){
                Serial.println(connectApprove);
                _connected = true;
                _connectionTimer.stop();
            }
        }
    }
}

} // namespace Arduino