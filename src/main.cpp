#include <Arduino.h>

#include "comport.h"
#include "camera.h"
#include "axis.h"

// #define TEST

#ifndef TEST
///// -- HARDWARE SETUP -- /////
const uint8_t xPulse = 2,
              xDir = 5,
              xEnable = 8,
              yPulse = 3,
              yDir = 6,
              yEnable = 8;
//////////////////////////////

using namespace Arduino;

Axis xAxis(300,1, xPulse, xDir, xEnable, 10),
     yAxis(300,1, yPulse, yDir, yEnable, 10);

ComPort port;
Camera camera(&xAxis, &yAxis);

void setup()
{
    port.init();
    port.subscribe(&camera);
    camera.setComPort(&port);

    volatile unsigned long start = 0,
                           end = 0,
                           delta = 0;
    while (true){
        start = micros();
        port.loopCheck();
        camera.loopCheck();
        end = micros();

        delta = end - start;
        if ( delta > 500 ) {
            Serial.print("loop takes: ");
            Serial.println(delta);
        }
    }
}

void loop() {}


#else

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    unsigned long currentTime1 = 0,
                  currentTime2 = 0;

    const unsigned long V0 = 10, // mm/sec
                   k = 10, //  step/mm
                   a = 40 * k; // step/sec2

    const unsigned long Vs0 = V0 * k; // step/sec
    unsigned long p = 0;
    unsigned long V, Vp = Vs0;

    currentTime1 = micros();
    for (long i = 0; i < 1; ++i) {
        V = (Vp + sqrt(Vp*Vp + 4*a))/2;
        p = 1000000 / V;
        Vp = V;
        // Serial.print("current period ");
        Serial.println(p);
    }
    currentTime2 = micros();

    Serial.print("calculation tooks ");
    Serial.print(currentTime2 - currentTime1);
    Serial.println(" mksec");
    Serial.println((int)sizeof(uint32_t));
    Serial.println((int)sizeof(uint64_t));
    Serial.println((int)sizeof(unsigned int));
    Serial.println((int)sizeof(unsigned long));
    Serial.println((int)sizeof(unsigned long long));
    delay(6000);
}
#endif
