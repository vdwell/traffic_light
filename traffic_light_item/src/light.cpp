#include "leds.h"
#include "light.h"
// #include <analogWrite.h>
#include <Arduino.h>

light::light(int lightID, int redLedPin, int greenLedPin, int yellowLed1Pin, int yellowLed2Pin, int buzzerPin)
    : red_led(redLedPin), green_led(greenLedPin), yellow1_led(yellowLed1Pin), yellow2_led(yellowLed2Pin)
{

    light_id = lightID;
    red_led_pin = redLedPin;
    green_led_pin = greenLedPin;
}

void light::trafficPermit(bool permit)
{
    if (permit)
    {
        green_led.turnOn();
        red_led.turnOff(false);
    }
    else
    {
        red_led.turnOn();
        green_led.turnOff(false);
        // yellow1_led.turnOff(false);
        // yellow2_led.turnOff(false);
    }
}

void light::speedLimit(int speedLimitation)
{
    if (speedLimitation == 0)
    {
        yellow1_led.turnOff(false);
        yellow2_led.turnOff(false);
        Serial.println("Speed NO_LIMIT");
    }
    else if (speedLimitation == 1)
    {
        yellow1_led.turnOn();
        yellow2_led.turnOff(false);
        Serial.println("Speed RESTRICTED");
    }
    else if (speedLimitation == 2)
    {
        yellow1_led.turnOn();
        yellow2_led.turnOn();
        Serial.println("Speed SLOW");
    }
}
