#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "wifiConecction.hpp"
WiFiClient wifiClient;
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip(192, 168, 0, 160);
IPAddress mqttBrokerIp(192, 168, 0, 71);
char *ssid = "Fibertel casaMi 2.4GHz_EXT";
char *password = "NEWELLS1974";

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Topic:" + String(topic));
  Serial.println("length:" + String(length));
  Serial.println(*payload);

  String content = "";
  for (uint8_t i = 0; i < length; i++)
  {
    content.concat((char)payload[i]);
  }
  Serial.println("content: " + content);
}
WiFiConnection wiFiConnection = WiFiConnection(ssid, password, ip, gateway, subnet);
PubSubClient mqttClient(mqttBrokerIp, 1883, callback, wifiClient);

bool MQTTconnectAndSubscribe()
{
  String hostName = wiFiConnection.getHostname();
  Serial.println(hostName);

  const bool mqttConected = mqttClient.connect(hostName.c_str());
  Serial.println("mqttConected: " + String(mqttConected));
  if (mqttConected)
  {
    Serial.println("mqtt");
    mqttClient.subscribe("hello/world", 1);
    mqttClient.publish("hello/world", "hello world", true);
  }
  return mqttConected;
}

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  wiFiConnection.connect();
  MQTTconnectAndSubscribe();
}

void loop()
{
  wiFiConnection.reconnectionHandler();
  if (!mqttClient.connected())
  {
    MQTTconnectAndSubscribe();
  }
  mqttClient.loop();
}
