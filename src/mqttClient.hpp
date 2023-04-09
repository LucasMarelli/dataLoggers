#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#ifndef MQTTClient_HPP
#define MQTTClient_HPP
class MQTTClient
{
private:
    static void callback(char *topic, byte *payload, unsigned int length)
    {
        Serial.println("Topic:" + String(topic));
        String content = "";
        for (uint8_t i = 0; i < length; i++)
        {
            content.concat((char)payload[i]);
        }
        Serial.println("content: " + content);
    }

public:
    PubSubClient mqttClient;
    String id;
    MQTTClient(String id_, IPAddress &mqttBrokerIp, int port, WiFiClient &wifiClient)
    {
        mqttClient = PubSubClient(mqttBrokerIp, port, callback, wifiClient);
        id = id_;
    }
    bool connectAndSubscribe()
    {
        Serial.println(id);
        const bool mqttConected = mqttClient.connect(id.c_str());
        if (mqttConected)
        {
            Serial.println("Conected with Broker!!");
            subscribe();
        }
        return mqttConected;
    }
    void subscribe()
    {
        mqttClient.subscribe("hello/world", 1);
        // mqttClient.publish("hello/world", "hello world", true);
    }

    bool reconnectionHandler()
    {
        if (!mqttClient.connected())
        {
            Serial.println("Broker Connection lost");
            return connectAndSubscribe();
        }
        return false;
    }
    void loop()
    {
        mqttClient.loop();
        reconnectionHandler();
    }
};
#endif