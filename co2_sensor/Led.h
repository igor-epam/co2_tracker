#pragma once

class Led
{
public:
    Led(int red, int green, int blue);
    virtual ~Led() = default;
    Led(Led const &) = delete;
    Led &operator=(Led const &) = delete;

    void setup();
    void loop();

    void set_color(int red, int green, int blue);

private:
    int const red_;
    int const green_;
    int const blue_;
};