#include "Lcd1602.h"

Lcd1602::Lcd1602(int i2c_address)
    : lcd_(i2c_address, Lcd1602Length, 2)
{
}

void Lcd1602::setup()
{
    lcd_.begin();
}

void Lcd1602::loop()
{
}

void Lcd1602::print(String l1, String l2)
{
    if (l1.length() > LcdLength)
        l1 = l1.substring(0, Lcd1602Length + 1);

    if (l2.length() > LcdLength)
        l2 = l2.substring(0, Lcd1602Length + 1);

    printLine(l1, 0);
    printLine(l2, 1);
}

void Lcd1602::printLine(String line, std::uint8_t index)
{
    lcd_.setCursor(0, index);
    buffer_.fill(' ');
    line.toCharArray(buffer_.data(), buffer_.size(), 0);
    lcd_.println(buffer_.data());
}