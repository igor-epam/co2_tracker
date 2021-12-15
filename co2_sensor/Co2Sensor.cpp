#include "Co2Sensor.h"

#include <HardwareSerial.h>

Co2Sensor::Co2Sensor(std::uint64_t update_delay, int rx, int tx)
    : MHZ19(rx, tx), millis_(millis()), update_delay_(update_delay)
{
}

void Co2Sensor::setup()
{
    setAutoCalibration(false);
    getStatus(); // 1-st query returns -1 always
    // while (isWarming())
    // {
    // }
}

void Co2Sensor::loop()
{
}

std::optional<int> Co2Sensor::getPPM()
{
    if (auto const now = millis(); now - millis_ > update_delay_)
    {
        Serial.println("getPPM");
        millis_ = now;
        return getMeasurement().co2_ppm;
    }
    return {};
}