#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "wifiConecction.hpp"
#include "mqttClient.hpp"
WiFiClient wifiClient;
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip(192, 168, 0, 160);
IPAddress mqttBrokerIp(192, 168, 0, 71);
char *ssid = "Fibertel casaMi 2.4GHz_EXT";
char *password = "NEWELLS1974";

WiFiConnection wiFiConnection = WiFiConnection(ssid, password, ip, gateway, subnet);
// PubSubClient mqttClient(mqttBrokerIp, 1883, callback, wifiClient);

MQTTClient mqttClient = MQTTClient(String(wiFiConnection.getHostname()), mqttBrokerIp, 1883, wifiClient);

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
