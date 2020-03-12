#include <Arduino.h>
#include <AStepper.h>

#include "comport.h"
#include "camera.h"

//PINS
const uint8_t xPulse = 0,
              xDir = 0,
              xSleep = 0,
              yPulse = 0,
              yDir = 0,
              ySleep = 0;


using namespace Arduino;

AStepper xMotor(1,2,3),
         yMotor(4,5,6);

ComPort port;
Camera camera(&xMotor, &yMotor);

void setup()
{
    port.subscribe(&camera);
    camera.setComPort(&port);
}

void loop() {
    port.loopCheck();
    camera.loopCheck();
}
