#include "Co2Handler.h"

#include "Mqtt.h"

Co2Handler::Co2Handler(Mqtt &mqtt, char const *state_topic)
    : mqtt_(mqtt), state_topic_(state_topic)
{
}

void Co2Handler::setup()
{
}

void Co2Handler::loop()
{
}

void Co2Handler::handle_value(std::optional<int> co2_level)
{
    if (co2_level)
    {
        auto const co2_as_string = String(co2_level.value());
        mqtt_.publish(state_topic_.c_str(), co2_as_string.c_str(), true /*retain*/);
    }
}