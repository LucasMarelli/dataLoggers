#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <DHT.h>
#include "pin.h"
#ifndef MQTTClient_HPP
#define MQTTClient_HPP
#define DHTPIN D3
#define DHTTYPE DHT11

extern NTPClient timeClient;

class MQTTClient
{
private:
    static int samplingTIme;
    int lastMeasurementMinutes = 0;
    static char CONFIG[30];
    static char MEASUREMENT[30];
    static char DEVICE_CONFIG[30];
    DHT dht = DHT(DHTPIN, DHTTYPE);

    static void callback(char *topic, byte *payload, unsigned int length)
    {
        Serial.println("Callback");
        Serial.println("Topic:" + String(topic));
        String content = "";
        for (unsigned int i = 0; i < length; i++)
        {
            content.concat((char)payload[i]);
        }
        Serial.println("content: " + content);
        // if (topic == CONFIG || topic == DEVICE_CONFIG)
        if (strcmp(topic, CONFIG) == 0 || strcmp(topic, DEVICE_CONFIG) == 0)
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
        dht.begin();
        id = id_;
        strcat(DEVICE_CONFIG, id.c_str());
        samplingTIme = 0;
    }
    bool connectAndSubscribe()
    {
        Serial.println(id);
        const bool mqttConected = mqttClient.connect(id.c_str());
        if (mqttConected)
        {
            Serial.println("Conected to Broker!!");
            subscribe();
        }
        return mqttConected;
    }
    void subscribe()
    {
        Serial.println(DEVICE_CONFIG);
        Serial.println(CONFIG);
        Serial.println(MEASUREMENT);
        mqttClient.subscribe(CONFIG, 1);
        mqttClient.subscribe(DEVICE_CONFIG, 1);
        Serial.println("Pass-Subscribe");
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
            return;

        float measurement = dht.readTemperature();

        const int minutes = timeClient.getMinutes();
        if (minutes % samplingTIme == 0 && lastMeasurementMinutes != minutes)
        {
            Serial.println("Publish Measurement: " + String(measurement));
            Serial.println("minutes " + String(minutes));
            Serial.println("topic " + String(MEASUREMENT));
            Serial.println("Medida " + String(timeClient.getEpochTime()));
            String payload = String(measurement);
            payload += "/";
            payload += String(timeClient.getEpochTime());
            mqttClient.publish(MEASUREMENT, payload.c_str());
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
char MQTTClient::CONFIG[30] = "device/config";
char MQTTClient::MEASUREMENT[30] = "device/measurement";
char MQTTClient::DEVICE_CONFIG[30] = "device/config/";
#endif