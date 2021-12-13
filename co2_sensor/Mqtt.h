#pragma once
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <LinkedList.h>

using CallbackFunction = std::function<void(byte *, unsigned int)>;

class Mqtt final
{
public:
  Mqtt(WiFiClient &wifi_client,
       char const *client_name,
       char const *born_topic);
  ~Mqtt() = default;
  Mqtt(Mqtt const &) = delete;
  Mqtt &operator=(Mqtt const &) = delete;

  void loop();
  void setup();
  void publish(char const *topic, char const *state, bool retain);
  void subscribe(char const *topic, CallbackFunction function);
  bool is_connected();

private:
  struct TopicAndSubscriber
  {
    String topic_;
    CallbackFunction function_;
  };

  void callback(char *topic, byte *payload, unsigned int length);
  void reconnect_if_needed();
  void just_born();

  String client_name_;
  String born_topic_;
  PubSubClient client_;
  LinkedList<TopicAndSubscriber> subscribers_;
};
