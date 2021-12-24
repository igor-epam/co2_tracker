#pragma once

#include <Arduino.h>

#include <optional>

class Mqtt;

class Co2Handler final
{
public:
    Co2Handler(Mqtt &mqtt, char const *state_topic);
    ~Co2Handler() = default;
    Co2Handler(Co2Handler const &) = delete;
    Co2Handler &operator=(Co2Handler const &) = delete;

    void setup();
    void loop();
    void handle_value(std::optional<int> co2_level);

private:
    Mqtt &mqtt_;
    String const state_topic_;
};