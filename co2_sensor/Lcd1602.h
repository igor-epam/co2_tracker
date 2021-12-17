#pragma once

#include <LiquidCrystal_I2C.h>
#include <array>

class Lcd1602 : private LiquidCrystal_I2C
{
public:
    Lcd1602(int i2c_address);
    ~Lcd1602() = default; // should be override
    Lcd1602(Lcd1602 const &) = delete;
    Lcd1602 &operator=(Lcd1602 const &) = delete;

    void setup();
    void loop();
    void print(String l1, String l2);
    void set_backlight(bool enable);

private:
    auto static constexpr Lcd1602Length = 16;
    void printLine(String line, std::uint8_t index);
    std::array<char, Lcd1602Length> buffer_;
};