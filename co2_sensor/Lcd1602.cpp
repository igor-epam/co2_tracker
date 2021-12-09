#include "Lcd1602.h"

#include <HardwareSerial.h>

Lcd1602::Lcd1602(int i2c_address)
    : LiquidCrystal_I2C(i2c_address, Lcd1602Length, 2)
{
}

void Lcd1602::setup()
{
    LiquidCrystal_I2C::begin();
}

void Lcd1602::loop()
{
}

void Lcd1602::print(String l1, String l2)
{
    if (l1.length() > Lcd1602Length)
        l1 = l1.substring(0, Lcd1602Length + 1);

    if (l2.length() > Lcd1602Length)
        l2 = l2.substring(0, Lcd1602Length + 1);

    printLine(l1, 0);
    printLine(l2, 1);
}

void Lcd1602::printLine(String line, std::uint8_t index)
{
    setCursor(0, index);
    buffer_.fill(' ');
    std::memcpy(buffer_.data(), line.c_str(), std::min(buffer_.size(), line.length()));
    Serial.write(buffer_.data(), buffer_.size());
    //Serial.println("<-- to lcd, original line size: " + String(line.length()) + ", buffer size : " + String(buffer_.size()));
    Print::write(buffer_.data(), buffer_.size());
}