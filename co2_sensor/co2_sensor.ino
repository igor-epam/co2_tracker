
#include "Co2Led.h"
#include "Lcd1602.h"
#include "WifiLocalClient.h"
#include "Mqtt.h"
#include "Co2Handler.h"

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

namespace pinouts
{
  auto constexpr RedPin = D7;
  auto constexpr GreenPin = D5;
  auto constexpr BluePin = D0;
  // used, but not configured, just hardcoded
  auto constexpr SdaPin = D2;
  auto constexpr SlcPin = D1;

  auto constexpr LcdAddress = 0x27;

  // strings
  auto constexpr MqttClientName = "co2_sensor";
  auto constexpr MqttWillTopic = "co2_sensor/alive";
  auto constexpr MqttStateTopic = "co2_sensor/state";
}

Co2Led led(pinouts::RedPin, pinouts::GreenPin, pinouts::BluePin);
Lcd1602 lcd(pinouts::LcdAddress);
WifiLocalClient wifi;
Mqtt mqtt(wifi.get_client(), pinouts::MqttClientName, pinouts::MqttWillTopic);
Co2Handler co2(mqtt, pinouts::MqttStateTopic);

static int was = 0;

namespace
{
  void just_test()
  {
    int delayMillis = 5000;
    auto const elapsed = millis() - was;
    static int counter;
    if (elapsed > delayMillis)
    {
      auto const co2_level = std::pow(counter % 10, 2) * 20;
      co2.handle_value(co2_level);
      lcd.print(String("counter: ") + String(counter++), "co2: " + String(co2_level));
      Serial.println("alive");
      if (co2_level < 600)
      {
        led.SetGreen();
      }
      else if (co2_level < 1000)
      {
        led.SetYellow();
      }
      else
      {
        led.SetRed();
      }
      was = millis();
    }
  }
}

void setup()
{
  //Serial.begin(9600);

  led.setup();
  led.SetGreen(); // by default

  lcd.setup();
  lcd.print(String("loading..."), "");

  wifi.setup();
  mqtt.setup();

  ArduinoOTA.setHostname("co2_tracker");
  ArduinoOTA.begin();
}

void loop()
{
  wifi.loop();
  mqtt.loop();
  led.loop();
  lcd.loop();
  ArduinoOTA.handle();
  just_test();
  // todo: make state display class
  if (!wifi.is_connected() || !mqtt.is_connected())
  {
    lcd.print("wifi connected: " + String(wifi.is_connected()), "mqtt connected: " + String(mqtt.is_connected()));
  }
}
