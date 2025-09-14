#include "joystick_as_button.h"
#include <Arduino.h>

JoystickAsButton::JoystickAsButton(int xpin, int ypin)
{
    joystickXPin = xpin;
    joystickYPin = ypin;
}

int JoystickAsButton::checkJoystickX()
{

    // return 0 = nothing to do
    // return 1 = short click
    // return 2 = long click

    xresult = 0;

    xstate = analogRead(joystickXPin);

    if (!xMovedRight && xstate > 924)
    {
        // joystick just has been moved Right
        joystickXMovedMS = millis();
        xMovedRight = true;
        xresult = 1;
    }
    else if (!xMovedLeft && xstate < 100)
    {
        // joystick just has been moved Left
        joystickXMovedMS = millis();
        xMovedLeft = true;
        xresult = -1;
    }

    if (xMovedRight && xstate < 612)
    {
        // joystick just has been moved to default original position from Right
        joystickXMovedMS = millis();
        xMovedRight = false;
        xresult = 0;
    }
    else if (xMovedLeft && xstate > 412)
    {
        // joystick just has been moved to default original position from Left
        joystickXMovedMS = millis();
        xMovedLeft = false;
        xresult = 0;
    }

    return xresult;
}

int JoystickAsButton::checkJoystickY()
{

    // return 0 = nothing to do
    // return 1 = short click
    // return 2 = long click

    yresult = 0;

    ystate = analogRead(joystickYPin);

    if (!yMovedDown && ystate > 924)
    {
        // joystick just has been moved down
        joystickYMovedMS = millis();
        yMovedDown = true;
        yresult = 1;
    }
    else if (!yMovedUp && ystate < 100)
    {
        // joystick just has been moved up
        joystickXMovedMS = millis();
        yMovedUp = true;
        yresult = -1;
    }

    if (yMovedDown && ystate < 612)
    {
        // joystick just has been moved to default original position from forward
        joystickYMovedMS = millis();
        yMovedDown = false;
        yresult = 0;
    }
    else if (yMovedUp && ystate > 412)
    {
        // joystick just has been moved to default original position from back
        joystickYMovedMS = millis();
        yMovedUp = false;
        yresult = 0;
    }

    return yresult;
}