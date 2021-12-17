#include "Co2Led.h"

Co2Led::Co2Led(int red, int green, int blue)
    : Led(red, green, blue) {}

void Co2Led::setup()
{
    Led::setup();
}

void Co2Led::loop()
{
    Led::loop();
}

void Co2Led::set_red()
{
    set_color(255, 0, 0);
}
void Co2Led::set_yellow()
{
    set_color(255, 75, 0);
}
void Co2Led::set_green()
{
    set_color(0, 255, 0);
}