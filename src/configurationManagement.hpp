#ifndef CONFIGURATION_MANAGEMENT_H
#define CONFIGURATION_MANAGEMENT_H
#include <WiFiClient.h>
#include <EEPROM.h>
#define EEPROM_SIZE 512
class ConfigurationManagement
{
private:
    static const int ELEMENT_MAX_SIZE = 30;
    const int WIFI_PASSWORD = 0;
    const int WIFI_SSID = WIFI_PASSWORD + ELEMENT_MAX_SIZE + 1;
    const int MQTT_IP = WIFI_SSID + ELEMENT_MAX_SIZE + 1;
    const int MQTT_PORT = MQTT_IP + ELEMENT_MAX_SIZE + 1;

    void getEEPROMData()
    {
        EEPROM.get(WIFI_PASSWORD, password);
        EEPROM.get(WIFI_SSID, ssid);
        EEPROM.get(MQTT_IP, mqttBrokerIp);
        EEPROM.get(MQTT_PORT, mqttBrokerPort);
    }

    void storeTest()
    {
        char pwd[ELEMENT_MAX_SIZE] = "123456";
        EEPROM.put(WIFI_PASSWORD, pwd);
        char ssid_[ELEMENT_MAX_SIZE] = "abc";
        EEPROM.put(WIFI_SSID, ssid_);
        int ip[4] = {255, 0, 0, 0};
        EEPROM.put(MQTT_IP, ip);
        int port = 1234;
        EEPROM.put(MQTT_PORT, port);
    }

public:
    int mqttBrokerIp[4] = {159, 65, 182, 177};
    int mqttBrokerPort = 1885;
    char ssid[ELEMENT_MAX_SIZE] = "SeVaLaVida 2.4GHz";
    char password[ELEMENT_MAX_SIZE] = "00436003571";
    ConfigurationManagement(/* args */)
    {
    }
    void init()
    {
       // EEPROM.begin(EEPROM_SIZE);
        // storeTest();
        // getEEPROMData();
    }
    // IPAddress getBrokerIp()
    // {
    //     return IPAddress(mqttBrokerIp[0], mqttBrokerIp[1], mqttBrokerIp[2], mqttBrokerIp[3]);
    // }
    int getBrokerPort()
    {
        return mqttBrokerPort;
    }
    char *getSsid()
    {
        return ssid;
    }
    char *getPassword()
    {
        return password;
    }

    void handleSerialData()
    {

        // getEEPROMData();
        if (Serial.available() > 0)
        {
        }
        // Serial.println(password);
        // Serial.println(ssid);
        // Serial.println(mqttBrokerPort);
        // int a = mqttBrokerIp[0];
        // Serial.println(a);
    }
};
#endif
