#pragma once

#include <ESP8266WiFi.h>
#include <functional>

class WifiLocalClient final
{
public:
  WifiLocalClient() = default;
  ~WifiLocalClient() = default;
  WifiLocalClient(WifiLocalClient const &) = delete;
  WifiLocalClient &operator=(WifiLocalClient const &) = delete;

  void loop();
  void setup();

  bool is_connected() const;
  WiFiClient &get_client();

private:
  void reconnect();
  String mac_to_string() const;

  WiFiClient wifi_client_;
};
