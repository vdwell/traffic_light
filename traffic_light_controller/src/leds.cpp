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

void leds::toggle()
{
    digitalWrite(pinLED, !digitalRead(pinLED));
}

void leds::turnOffAfterDelay(int millisec, bool analogOff)
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisTurnOff <= millisec)
    {
        // we do not reset previousMillisTurnOff
    }
    else
    {
        turnOff(analogOff);
    }
}

void leds::fadeOn(int millisec)
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisFadeOn >= millisec)
    {
        previousMillisFadeOn = currentMillis;
        if (brightness <= 255)
        {
            Serial.println(brightness);
            analogWrite(pinLED, brightness);
            brightness += fadeAmount;
        }
    }
}

void leds::pulsate(int delay_ms)
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisPulsate >= delay_ms)
    {
        previousMillisPulsate = currentMillis;
        brightness += fadeAmount;
        if (brightness == 0 || brightness == 30)
        {
            fadeAmount = -fadeAmount;
        }
        analogWrite(pinLED, brightness);
    }
}

void leds::blinkNTimes(int number_of_times, int blink_ms, int interval_ms)
{
    // unsigned long currentMillis = millis();
    if (BlinkNTimesCounter < number_of_times)
    {
        // if led is off and it is time to blink
        if (!digitalRead(pinLED) && millis() - previousMillisBlinkNTimes >= interval_ms)
        {
            previousMillisBlinkNTimes = millis();
            digitalWrite(pinLED, HIGH);

            // if led is on and it is time to off
        }
        else if (digitalRead(pinLED) && millis() - previousMillisBlinkNTimes >= blink_ms)
        {
            previousMillisBlinkNTimes = millis();
            digitalWrite(pinLED, LOW);
            BlinkNTimesCounter++;
        }
    }
    else
    {
        BlinkNTimesCounter = 0;
    }
}

/*
void leds::blinkNTimes(int number_of_times, int blink_time, int time_between_blinks) {
 counter = 0;
 blink_flag = false;
 pause_flag = fase;
 myTimer = millis();
 if (millis() - myTimer >= time_between_blinks) {
  blink_flag = true;
  digitalWrite(pinLED, HIGH);
 }
 else if (millis() - myTimer >= blink_time) {

 }
}
*/