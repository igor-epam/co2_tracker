#include "Co2Led.h"
#include "Lcd1602.h"
#include "WifiLocalClient.h"
#include "Mqtt.h"
#include "Co2Handler.h"
#include "Co2Sensor.h"

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <string>

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
  auto constexpr MqttNightModeTopic = "co2_sensor/night_mode";

}

namespace
{
  class DisplayState
  {
  public:
    DisplayState(Co2Led &led, Lcd1602 &lcd)
        : led_(led), lcd_(lcd)
    {
    }
    DisplayState(DisplayState const &) = delete;
    DisplayState operator=(DisplayState const &) = delete;
    void loop()
    {
    }
    void setup()
    {
      lcd_.print(String("loading..."), "");
    }
    void handle(std::optional<int> co2_level_optional, bool wifi_ok, bool mqtt_ok)
    {
      if ((wifi_ok == wifi_ok_) && (mqtt_ok == mqtt_ok_) && (!co2_level_optional || (co2_level_optional == co2_level_)))
        return;

      String first_line = "co2: ";
      String second_line = "ok";
      auto const manage_co2_level = [&]()
      {
        auto const value_to_use = co2_level_optional ? co2_level_optional : co2_level_;
        if (!value_to_use)
        {
          first_line += "undefined";
          return;
        }

        auto const co2_level = value_to_use.value();
        first_line += String(co2_level) + " ppm";
        if (co2_level < 600)
        {
          led_.set_green();
        }
        else if (co2_level < 1000)
        {
          led_.set_yellow();
          second_line = String("open window");
        }
        else
        {
          led_.set_red();
          second_line = String("open window ASAP");
        }

        co2_level_ = value_to_use;
      };
      manage_co2_level();
      if ((wifi_ok != wifi_ok_) || (mqtt_ok != mqtt_ok_))
      {
        auto const bool_to_string = [](auto bool_value) -> String
        {
          return bool_value ? "ok" : "x";
        };
        second_line = "w: " + bool_to_string(wifi_ok) + ", m: " + bool_to_string(mqtt_ok);
        wifi_ok_ = wifi_ok;
        mqtt_ok_ = mqtt_ok;
      }
      lcd_.print(first_line, second_line);
    }

  private:
    Co2Led &led_;
    Lcd1602 &lcd_;
    std::optional<int> co2_level_ = {std::nullopt};
    std::optional<bool> wifi_ok_ = {std::nullopt};
    std::optional<bool> mqtt_ok_ = {std::nullopt};
  };
}

Co2Led led(pinouts::RedPin, pinouts::GreenPin, pinouts::BluePin);
Lcd1602 lcd(pinouts::LcdAddress);
WifiLocalClient wifi;
Mqtt mqtt(wifi.get_client(), pinouts::MqttClientName, pinouts::MqttWillTopic);
Co2Handler co2(mqtt, pinouts::MqttStateTopic);
Co2Sensor co2_sensor(30 * 1000 /*ms*/, pinouts::RXPin, pinouts::TXPin);
DisplayState display_state(led, lcd);

void setup()
{
  //Serial.begin(9600);

  led.setup();
  led.set_green(); // by default

  lcd.setup();

  co2_sensor.setup();

  wifi.setup();
  mqtt.setup();
  mqtt.subscribe(pinouts::MqttCalibrateTopic, [&](byte *, unsigned int)
                 { co2_sensor.calibrate(); });

  mqtt.subscribe(pinouts::MqttNightModeTopic, [&](byte *payload, unsigned int length)
                 {
                   auto const is_night = "on" == std::string(reinterpret_cast<char const *>(payload), length);
                   auto const enable = !is_night;
                   led.enable(enable);
                   lcd.set_backlight(enable);
                 });
  display_state.setup();

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
  co2.loop();
  auto const co2_optional_value = co2_sensor.getPPM();
  co2.handle_value(co2_optional_value);
  display_state.handle(co2_optional_value, wifi.is_connected(), mqtt.is_connected());
}
