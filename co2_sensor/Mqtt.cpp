#include "Mqtt.h"

namespace
{
    const char *MQTT_SERVER = "192.168.100.236";
    int MQTT_PORT = 1883;
    const char *MQTT_USER = "";
    const char *MQTT_PASSWORD = "";
}

Mqtt::Mqtt(WiFiClient &wifi_client, char const *client_name, char const *born_topic)
    : client_name_(client_name), born_topic_(born_topic), client_(wifi_client)
{
}

void Mqtt::loop()
{
    reconnect_if_needed();
    client_.loop();
}

void Mqtt::setup()
{
    Serial.println("mqtt setup");
    client_.setServer(MQTT_SERVER, MQTT_PORT);
    client_.setCallback([&](char *topic, byte *payload, unsigned int length)
                        { this->callback(topic, payload, length); });
    Serial.println("mqtt setup end");
}

void Mqtt::publish(char const *topic, char const *state, bool retain)
{
    String message = "publish topic: ";
    message += topic;
    message += ", state ";
    message += state;
    Serial.println(message);
    client_.publish(topic, reinterpret_cast<const std::uint8_t *>(state), strlen(state), retain);
}

void Mqtt::subscribe(char const *topic, CallbackFunction function)
{
    subscribers_.add(TopicAndSubscriber{topic, std::move(function)});
    client_.subscribe(topic);
}

void Mqtt::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("got callback for topic ");
    Serial.println(topic);
    auto const list_size = subscribers_.size();
    for (auto i = 0; i < list_size; i++)
    {
        auto const &subscriber = subscribers_.get(i);
        auto const subscriber_topic_length = subscriber.topic_.length();
        if (subscriber_topic_length == strlen(topic) && !memcmp(topic, subscriber.topic_.c_str(), subscriber_topic_length))
        {
            subscriber.function_(payload, length);
        }
    }
}

void Mqtt::reconnect_if_needed()
{
    if (client_.connected())
        return;

    Serial.println("mqtt reconnect");
    auto started = millis();
    client_.connect(client_name_.c_str(), MQTT_USER, MQTT_PASSWORD, born_topic_.c_str(), 2, true /*retain*/, "offline");
    while (!client_.connected())
    {
        delay(500); //wait for 500 ms
        Serial.println("trying to reconnect mqtt");
        auto const now = millis();
        if (now < started)
        {
            started = now;
        }
        if (now - started > 5 /*sec*/ * 1000 /*ms*/)
        {
            Serial.println("Tineout while reconnecting to mqtt");
            return;
        }
    }
    just_born();
}

void Mqtt::just_born()
{
    Serial.println("just born");
    auto const list_size = subscribers_.size();
    for (auto i = 0; i < list_size; i++)
    {
        auto const &subscriber = subscribers_.get(i);
        client_.subscribe(subscriber.topic_.c_str());
    }
    if (born_topic_.length())
    {
        Serial.println("send born message");
        client_.publish(born_topic_.c_str(), reinterpret_cast<const std::uint8_t *>("online"), 6 /*size*/, true /*retain*/);
    }
}
