
#include "Co2Led.h"
#include "Lcd1602.h"
#include "WifiLocalClient.h"
#include "Mqtt.h"

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
}

Co2Led led(pinouts::RedPin, pinouts::GreenPin, pinouts::BluePin);
Lcd1602 lcd(pinouts::LcdAddress);
WifiLocalClient wifi;
Mqtt mqtt(wifi.get_client(), pinouts::MqttClientName, pinouts::MqttWillTopic);

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
      lcd.print(String("alive ") + String(counter++), "");
      Serial.println("alive");
      was = millis();
    }
  }
}

void setup()
{
  Serial.begin(9600);

  led.setup();

  lcd.setup();
  led.SetGreen();

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
}
