#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#ifndef MQTTClient_HPP
#define MQTTClient_HPP
#include <NTPClient.h>

extern NTPClient timeClient;

class MQTTClient
{
private:
    static int samplingTIme;
    int lastMeasurementMinutes = 0;
    static struct Topics
    {
        String CONFIG;
        String DEVICE_CONFIG;
        String MEASUREMENT;
    } Topic;

    static void callback(char *topic, byte *payload, unsigned int length)
    {
        Serial.println("Topic:" + String(topic));
        String content = "";
        for (uint8_t i = 0; i < length; i++)
        {
            content.concat((char)payload[i]);
        }
        Serial.println("content: " + content);
        if (String(topic) == Topic.CONFIG || String(topic) == Topic.DEVICE_CONFIG)
        {
            samplingTIme = content.toInt();
            Serial.println("SamplingTime Changed to " + String(samplingTIme));
        }
        else
        {
            Serial.println("Event not handled");
        }
    }

public:
    PubSubClient mqttClient;
    String id;
    MQTTClient(String id_, IPAddress &mqttBrokerIp, int port, WiFiClient &wifiClient)
    {
        mqttClient = PubSubClient(mqttBrokerIp, port, callback, wifiClient);
        id = id_;
        Topic.CONFIG = "device/config";
        Topic.MEASUREMENT = "device/measurement";
        Topic.DEVICE_CONFIG = "device/config/";
        Topic.DEVICE_CONFIG += String(id_);
        samplingTIme = 0;
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
        Serial.println(Topic.DEVICE_CONFIG);
        Serial.println(Topic.CONFIG);
        Serial.println(Topic.MEASUREMENT);
        mqttClient.subscribe(Topic.CONFIG.c_str(), 1);
        mqttClient.subscribe(Topic.DEVICE_CONFIG.c_str(), 1);
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

    void handleMeassurement()
    {
        if (!samplingTIme)
            return randomSeed(10);

        float measurement = random(100, 350) / 10;
        // float measurement = 25;
        const int minutes = timeClient.getMinutes();
        if (minutes % samplingTIme == 0 && lastMeasurementMinutes != minutes)
        {
            Serial.println("Publish Measurement: " + String(measurement));
            Serial.println("minutes " + String(minutes));
            Serial.println("topic " + String(Topic.MEASUREMENT));
            mqttClient.publish(Topic.MEASUREMENT.c_str(), String(measurement).c_str());
            lastMeasurementMinutes = minutes;
        }
    }

    void loop()
    {
        handleMeassurement();
        mqttClient.loop();
        reconnectionHandler();
    }
};
// Definición de variables estáticas
int MQTTClient::samplingTIme = 0;
MQTTClient::Topics MQTTClient::Topic = {"                                   ", "                                   ", "                                   "};
#endif