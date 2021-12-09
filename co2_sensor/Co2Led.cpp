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

void Co2Led::SetRed()
{
    set_color(255, 0, 0);
}
void Co2Led::SetYellow()
{
    set_color(255, 255, 0);
}
void Co2Led::SetGreen()
{
    set_color(0, 255, 0);
}
void Co2Led::SetBrown()
{
    set_color(150, 75, 0);
}