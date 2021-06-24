#include <Arduino.h>

// #define TEST

#ifndef TEST

#include "comport.h"
#include "camera.h"
#include "axis.h"
#include "pump.h"

///// -- HARDWARE SETUP -- /////
const uint8_t xPulse = 2,
              xDir = 5,
              xEnable = 8,
              xEndstop = 9,

              yPulse = 3,
              yDir = 6,
              yEnable = 8,
              yEndstop = 12,

              backlight = 10,

              iodinePumpPin = 11,
              chlorinePumpPin = 4;
//////////////////////////////

using namespace Arduino;

Axis xAxis(10, xPulse, xDir, xEnable, xEndstop, EndstopAtMax);
Axis yAxis(10, yPulse, yDir, yEnable, yEndstop, EndstopAtMax);

ComPort port;
Camera camera(&xAxis, &yAxis, backlight);

Pump iodinePump(iodinePumpSign, iodinePumpPin, &port);
Pump chlorinePump(chlorinePumpSign, chlorinePumpPin, &port);


void setup()
// int main()
{
    //Axis setup
    xAxis.setMaxPos(xMaxPos);
    xAxis.setSpeed(150, 2, 50);

    yAxis.setMaxPos(yMaxPos);
    yAxis.setSpeed(150, 2, 50);
    yAxis.invertDirection();
    /////

    port.init();
    port.subscribe(&camera);
    port.subscribe(&iodinePump);
    port.subscribe(&chlorinePump);
    camera.setComPort(&port);

    // volatile unsigned long start = 0,
                        //    end = 0,
                        //    delta = 0;
    while (true){
        // start = micros();

        port.loopCheck();

        if ( iodinePump.isBusy() || chlorinePump.isBusy() ) 
        {
            iodinePump.loop();
            chlorinePump.loop();
        } 
        else camera.loopCheck();

        // end = micros();
        // delta = end - start;
        // if ( delta > 500 ) {
        //     Serial.print("lptks: ");
        //     Serial.println(delta);
        // }
        // delay(100);
    }
    // return 0;
}

void loop() {}


#else


void setup()
{
    // Serial.begin(9600);
    pinMode(10,OUTPUT);
    // digitalWrite(10,HIGH);

// Пины D9 и D10 - 30 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00000100;  // x256 phase correct

// Пины D9 и D10 - 7.5 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00000101;  // x1024 phase correct

// Пины D9 и D10 - 7.8 кГц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00000001;  // x1 phase correct

// Пины D9 и D10 - 15.6 кГц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00001001;  // x1 fast pwm

// Пины D9 и D10 - 122 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00000011;  // x64 phase correct

// Пины D9 и D10 - 61 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00001100;  // x256 fast pwm

// Пины D9 и D10 - 2 кГц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00001010;  // x8 fast pwm

// Пины D9 и D10 - 977 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00000010;  // x8 phase correct

// Пины D9 и D10 - 244 Гц 10bit
// TCCR1A = 0b00000011;  // 10bit
// TCCR1B = 0b00001011;  // x64 fast pwm
}

int step = 1;

void loop()
{
    static int level = 0;
    if (level != 250) {
        analogWrite(10,level);
        level += step;
    }
    // if (level == 0 || level == 20)
        // step = -step;

// digitalWrite(10,HIGH);
    delay(30);

    // digitalWrite(10,LOW);
    // delay(1000);
}
#endif
