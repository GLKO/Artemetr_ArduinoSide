#include <Arduino.h>
#include <ATimer.h>
#include <keywords.h>
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

ATimer connectionTimer,
       positionUpdateTimer;

bool connected = false;

AStepper xMotor(1,2,3),
         yMotor(4,5,6);
ComPort port;
Camera camera(&xMotor, &yMotor);

void setup() {

}

void loop() {

    if (connectionTimer.check() && !connected) {
        Serial.println(movingCamId);

        if (Serial.find(connectRequest)){
            Serial.println(connectApprove);
            connected = true;
            connectionTimer.stop();
        }
    }
}

// void serialEvent() {
//   while (Serial.available()) {
//     // get the new byte:
//     char inChar = (char)Serial.read();
//     // add it to the inputString:
//     inputString += inChar;
//     // if the incoming character is a newline, set a flag so the main loop can
//     // do something about it:
//     if (inChar == '\n') {
//       stringComplete = true;
//     }
//   }
// }