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
    Serial.setTimeout(500);
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
    // volatile auto start = micros();
    if ( _connected ) {
        if ( Serial.available() == commandSize ) {
            Serial.readBytes(message, commandSize);
            // Serial.println(message);
            notifySubscribers();
        }
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
    // volatile auto end = micros();
    // Serial.print("ComPort::loopCheck takes ");
    // Serial.println(end-start);
}

} // namespace Arduino