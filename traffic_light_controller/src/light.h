#ifndef light_h
#define light_h

#include <Arduino.h>
#include "leds.h"

class light
{

private:
    int light_id;
    int red_led_pin;
    int green_led_pin;
    leds red_led;
    leds green_led;
    leds yellow1_led;
    leds yellow2_led;
    int speedLimitation;

public:
    // enum speed
    // {
    //   NO_LIMIT = 0,
    //   RESTRICTED = 1,
    //   SLOW = 2

    // } speed_limitation;

    light(int lightID, int redLedPin, int greenLedPin, int yellowLed1Pin, int yellowLed2Pin, int buzzerPin);
    void trafficPermit(bool permit);
    void speedLimit(int speedLimitation);
    void setLightId(int lightID);
};

#endif