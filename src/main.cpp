#include <Arduino.h>
#include <ATimer.h>
#include <keywords.h>

ATimer connectionTimer,
       positionUpdateTimer;

bool connected = false;

void setup() {
    Serial.begin(connnectionSpeed);
    connectionTimer.setPeriod(1000);
    connectionTimer.start();
    positionUpdateTimer.setPeriod(200);
    positionUpdateTimer.start();
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

    if (positionUpdateTimer.check() && connected) {
        char *q = "NEW_POS ";
        String message(q);
        message += String(rand());
        message += ' ';
        message += String(rand());

        Serial.println(message);
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