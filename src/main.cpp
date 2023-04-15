#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "wifiConecction.hpp"
#include "mqttClient.hpp"
WiFiClient wifiClient;
IPAddress mqttBrokerIp(192, 168, 0, 71);
char *ssid = "Fibertel casaMi 2.4GHz_EXT";
char *password = "NEWELLS1974";

WiFiConnection wiFiConnection = WiFiConnection(ssid, password);
// PubSubClient mqttClient(mqttBrokerIp, 1883, callback, wifiClient);

MQTTClient mqttClient = MQTTClient(String(wiFiConnection.getHostname()), mqttBrokerIp, 1887, wifiClient);

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  wiFiConnection.connect();
  mqttClient.connectAndSubscribe();
}

void loop()
{
  wiFiConnection.reconnectionHandler();
  mqttClient.loop();
}
