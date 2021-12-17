#include "Led.h"

#include "Arduino.h"

Led::Led(int red, int green, int blue)
    : enabled_(true), red_(red), green_(green), blue_(blue), red_value_(0), green_value_(0), blue_value_(0)
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
    if (enabled_)
    {
        analogWrite(red_, red);
        analogWrite(green_, green);
        analogWrite(blue_, blue);
    }
    red_value_ = red;
    green_value_ = green;
    blue_value_ = blue;
}

void Led::enable(bool enable)
{
    enabled_ = enable;
    if (enabled_)
    {
        //restore state
        analogWrite(red_, red_value_);
        analogWrite(green_, green_value_);
        analogWrite(blue_, blue_value_);
    }
    else
    {
        analogWrite(red_, 0);
        analogWrite(green_, 0);
        analogWrite(blue_, 0);
    }
}