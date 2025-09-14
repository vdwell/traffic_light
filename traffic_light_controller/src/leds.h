#ifndef leds_h
#define leds_h

#include <Arduino.h>

class leds
{

private:
    int pinLED;
    int brightness = 0;
    int fadeAmount = 5;
    bool pulsateMode = true;
    unsigned long prviousMillis = 0;
    unsigned long previousMillisTurnOff = 0;
    unsigned long previousMillisPulsate = 0;
    unsigned long previousMillisFadeOn = 0;
    unsigned long previousMillisFadeOff = 0;
    int BlinkNTimesCounter = 0;
    unsigned long previousMillisBlinkNTimes = 0;

public:
    leds(int pin);
    void turnOn();
    void turnOff(bool analogOff);
    void fadeOn(int millisec);
    void toggle();
    void flash(int delay_ms);
    void turnOffAfterDelay(int millisec, bool analogOff);
    void pulsate(int delay_ms);
    void setPulsateMode(bool val);
    void blinkNTimes(int number_of_times, int blink_ms, int interval_ms);
};

#endif