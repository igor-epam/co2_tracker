#pragma once

#include <LiquidCrystal_I2C.h>
#include <array>

auto constexpr Lcd1602Length = 16;

class Lcd1602 final
{
public:
    Lcd1602(int i2c_address);
    ~Lcd1602() = default;
    Lcd1602(Lcd1602 const &) = delete;
    Lcd1602 &operator=(Lcd1602 const &) = delete;

    void setup();
    void loop();
    void print(String l1, String l2);

private:
    void printLine(String line, std::uint8_t index);

    std::size_t static LcdLength;

    LiquidCrystal_I2C lcd_;
    std::array<char, Lcd1602Length> buffer_;
};