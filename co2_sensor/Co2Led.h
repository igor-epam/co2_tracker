#pragma once

#include "Led.h"

class Co2Led : private Led
{

public:
    Co2Led(int red, int green, int blue);
    ~Co2Led() override = default;
    Co2Led(Co2Led const &) = delete;
    Co2Led &operator=(Co2Led const &) = delete;

    void setup();
    void loop();

    using Led::enable;
    void set_red();
    void set_yellow();
    void set_green();
};