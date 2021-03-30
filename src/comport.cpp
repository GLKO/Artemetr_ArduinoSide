#include "comport.h"
#include <Arduino.h>

namespace Arduino
{

ComPort::ComPort()
    : _connectionTimer(1000)
{
    _connectionTimer.start();
}

void ComPort::init()
{
    Serial.end();
    Serial.begin(connnectionSpeed);
    Serial.setTimeout(100);
    // Serial.setTimeout(0);
}

void ComPort::sendMessage(const char *msg)
{
    if ( _connected) {
        Serial.write(msg, commandSize);
        // Serial.write('\n');
    }
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
            if ( *message == 'C' ) {
                _connected = false;
                _connectionTimer.start();
                Serial.setTimeout(100);
            } 
            else {
                notifySubscribers();
            }
        }
    } 
    else {
        if (_connectionTimer.timeout() ) {
            Serial.println(movingCamId);

            if (Serial.find(connectRequest)){
                Serial.println(connectApprove);
                _connected = true;
                _connectionTimer.stop();
                while (Serial.read() != -1) 
                {}
                Serial.setTimeout(0);
            }
        }
    }
    // volatile auto end = micros();
    // Serial.print("ComPort::loopCheck takes ");
    // Serial.println(end-start);
}

} // namespace Arduino
