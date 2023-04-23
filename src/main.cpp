#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "wifiConecction.hpp"
#include "mqttClient.hpp"
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiClient wifiClient;
IPAddress mqttBrokerIp(192, 168, 0, 58);
// char *ssid = "Fibertel casaMi 2.4GHz_EXT";
// char *password = "NEWELLS1974";
char *ssid = "SeVaLaVida 2.4GHz";
char *password = "00436003571";

WiFiConnection wiFiConnection = WiFiConnection(ssid, password);
// PubSubClient mqttClient(mqttBrokerIp, 1883, callback, wifiClient);

MQTTClient mqttClient = MQTTClient(String(wiFiConnection.getHostname()), mqttBrokerIp, 1885, wifiClient);

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP);

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  wiFiConnection.connect();
  mqttClient.connectAndSubscribe();
  timeClient.begin();
}

void loop()
{
  timeClient.update();
  wiFiConnection.reconnectionHandler();
  mqttClient.loop();
}
