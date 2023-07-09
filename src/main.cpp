// https://github.com/esp8266/Arduino/discussions/8160

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "wifiConecction.hpp"
#include "mqttClient.hpp"
#include <WiFiUdp.h>
#include <NTPClient.h>
WiFiClient wifiClient;
IPAddress mqttBrokerIp(159, 65, 182, 177);
// char *ssid = "Fibertel casaMi 2.4GHz_EXT";
// char *password = "mirdanluc1";

char ssid[30] = "SeVaLaVida 2.4GHz";
char password[30] = "00436003571";

WiFiConnection wiFiConnection = WiFiConnection(ssid, password);
MQTTClient mqttClient = MQTTClient(String(wiFiConnection.getHostname()), mqttBrokerIp, 1885, wifiClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  while (!Serial)
    ;
  wiFiConnection.connect();
  mqttClient.connectAndSubscribe();
  timeClient.begin();
}

void loop()
{
  // Serial.println("Loop");
  timeClient.update();
  wiFiConnection.reconnectionHandler();
  mqttClient.loop();
  // Serial.println("Loop-before-Serial.available");
  // if (Serial.available())
  // {
  // }
  // Serial.println("Loop-after-Serial.available");
}
