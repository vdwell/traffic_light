#ifndef JoystickAsButton_h
#define JoystickAsButton_h

#include <Arduino.h>

class JoystickAsButton
{
protected:
    int joystickXPin;
    int joystickYPin;
    int xstate;
    int xresult;
    int ystate;
    int yresult;
    unsigned long joystickXMovedMS;
    unsigned long joystickYMovedMS;
    bool xMovedLeft = false;
    bool xMovedRight = false;
    bool yMovedUp = false;
    bool yMovedDown = false;

public:
    JoystickAsButton(int xpin, int ypin);
    int checkJoystickX();
    int checkJoystickY();
};

#endif