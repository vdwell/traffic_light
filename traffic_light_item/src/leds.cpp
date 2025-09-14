#include "leds.h"
// #include <analogWrite.h>
#include <Arduino.h>

leds::leds(int pin)
{
    pinLED = pin;
    pinMode(pinLED, OUTPUT);
}
void leds::turnOn()
{
    digitalWrite(pinLED, HIGH);
}

void leds::turnOff(bool analogOff)
{
    if (analogOff)
    {
        analogWrite(pinLED, 0);
    }
    else
    {
        digitalWrite(pinLED, LOW);
    }
}