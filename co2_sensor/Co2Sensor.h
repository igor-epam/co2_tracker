#pragma once

#include <MHZ19.h>

#include <optional>

class Co2Sensor : private MHZ19
{
public:
    Co2Sensor(std::uint64_t update_delay, int rx, int tx);
    ~Co2Sensor() override = default;
    Co2Sensor(Co2Sensor const &) = delete;
    Co2Sensor &operator=(Co2Sensor const &) = delete;

    void setup();
    void loop();

    std::optional<int> getPPM();

    void calibrate();

private:
    unsigned long millis_;
    std::uint64_t const update_delay_;
};