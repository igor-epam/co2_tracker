#include "WifiLocalClient.h"

namespace
{
    const char *WIFI_SSID = "";
    const char *WIFI_PASSWORD = "";
}

void WifiLocalClient::loop()
{
    if (!is_connected())
    {
        reconnect();
    }
}

void WifiLocalClient::setup()
{
    //WiFi.setAutoReconnect(true);
    //WiFi.persistent(true);
}
bool WifiLocalClient::is_connected() const
{
    auto const status = WiFi.status();
    //Serial.print("Wifi status ");
    //Serial.println(status);
    return WiFi.status() == WL_CONNECTED;
}

WiFiClient &WifiLocalClient::get_client()
{
    return wifi_client_;
}

void WifiLocalClient::reconnect()
{
    Serial.print("Reconnecting to ");
    Serial.println(WIFI_SSID);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.waitForConnectResult();
    if (!is_connected())
    {
        Serial.println("Tineout while reconnecting to wifi");
        return;
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(mac_to_string());
    Serial.println(WiFi.localIP());
}

String WifiLocalClient::mac_to_string() const
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String result;
    for (int i = 0; i < 6; ++i)
    {
        result += String(mac[i], 16);
        if (i < 5)
            result += ':';
    }
    return result;
}
