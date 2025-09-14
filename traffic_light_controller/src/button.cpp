#include "button.h"
#include <Arduino.h>

Button::Button(int pin)
{
    btnPin = pin;
}
void Button::start()
{
    pinMode(btnPin, INPUT_PULLUP);
    state = HIGH;
}
int Button::checkBTN()
{

    // return 0 = nothing to do
    // return 1 = short click
    // return 2 = long click

    result = 0;

    int prevState = state;
    state = digitalRead(btnPin);
    if (prevState == HIGH && state == LOW)
    {
        // button just has been pressed
        buttonClickedMS = millis();
        result = 0;
    }
    else if (prevState == LOW && state == HIGH)
    {
        // button just has been released
        result = 0;

        if (millis() - buttonClickedMS < 50)
        {
            // debounce area - nothing to do
        }
        else if (millis() - buttonClickedMS >= 50 && millis() - buttonClickedMS < 1000)
        {
            // button was released after 50ms but before 200ms after pressing = short click
            result = 1;
        }
        else if (millis() - buttonClickedMS >= 1000 && millis() - buttonClickedMS < 10000)
        {
            // button was released after 200ms but before 900ms after pressing = long click
            result = 2;
        }
    }
    return result;
}