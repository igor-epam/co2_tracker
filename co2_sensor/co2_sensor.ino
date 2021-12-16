
#include "Co2Led.h"
#include "Lcd1602.h"
#include "WifiLocalClient.h"
#include "Mqtt.h"
#include "Co2Handler.h"
#include "Co2Sensor.h"

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

  auto constexpr RXPin = RX;
  auto constexpr TXPin = TX;

  auto constexpr LcdAddress = 0x27;

  // strings
  auto constexpr MqttClientName = "co2_sensor";
  auto constexpr MqttWillTopic = "co2_sensor/alive";
  auto constexpr MqttStateTopic = "co2_sensor/state";
  auto constexpr MqttCalibrateTopic = "co2_sensor/calibrate";

}

Co2Led led(pinouts::RedPin, pinouts::GreenPin, pinouts::BluePin);
Lcd1602 lcd(pinouts::LcdAddress);
WifiLocalClient wifi;
Mqtt mqtt(wifi.get_client(), pinouts::MqttClientName, pinouts::MqttWillTopic);
Co2Handler co2(mqtt, pinouts::MqttStateTopic);
Co2Sensor co2_sensor(30 * 1000 /*ms*/, pinouts::RXPin, pinouts::TXPin);

namespace
{
  void display(std::optional<int> co2_level_optional, bool wifi_ok, bool mqtt_ok)
  {
    if (!co2_level_optional)
      return;

    auto const co2_level = co2_level_optional.value();
    co2.handle_value(co2_level);
    Serial.println("alive");
    String first_line = "co2: " + String(co2_level) + " PPM";
    String second_line = "ok";
    if (co2_level < 600)
    {
      led.SetGreen();
    }
    else if (co2_level < 1000)
    {
      led.SetYellow();
      second_line = "open window";
    }
    else
    {
      led.SetRed();
      second_line = "open window ASAP";
    }
    if (!wifi_ok || !mqtt_ok)
    {
      second_line = "w?:" + String(wifi_ok) + ",m?:" + String(mqtt_ok);
    }

    lcd.print(first_line, second_line);
  }
}

void setup()
{
  //Serial.begin(9600);

  led.setup();
  led.SetGreen(); // by default

  lcd.setup();
  lcd.print(String("loading..."), "");

  co2_sensor.setup();

  wifi.setup();
  mqtt.setup();
  mqtt.subscribe(pinouts::MqttCalibrateTopic, [&](byte *, unsigned int)
                 { co2_sensor.calibrate(); });

  ArduinoOTA.setHostname("co2_tracker");
  ArduinoOTA.begin();
}

void loop()
{
  wifi.loop();
  mqtt.loop();
  led.loop();
  lcd.loop();
  co2_sensor.loop();
  ArduinoOTA.handle();
  co2_sensor.loop();
  auto const co2_level = co2_sensor.getPPM();

  if (co2_level)
  {
    display(co2_level.value(), wifi.is_connected(), mqtt.is_connected());
  }
}
