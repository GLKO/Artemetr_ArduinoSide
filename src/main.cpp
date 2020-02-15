#include <Arduino.h>
// #include <string.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    String str = "current time: ";
    str += String(millis()/1000.0,1);
    // char* message = "qwerty";
    // str += message;
    // Serial.write(str.c_str());
    Serial.println(str);
    delay(1417);
}