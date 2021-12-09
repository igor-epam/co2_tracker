#include "Led.h"

#include "Arduino.h"

Led::Led(int red, int green, int blue)
    : red_(red), green_(green), blue_(blue)
{
}

void Led::setup()
{
    pinMode(red_, OUTPUT);
    pinMode(green_, OUTPUT);
    pinMode(blue_, OUTPUT);
}

void Led::loop()
{
}

void Led::set_color(int red, int green, int blue)
{
    analogWrite(red_, red);
    analogWrite(green_, green);
    analogWrite(blue_, blue);
}