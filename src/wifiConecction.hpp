#include <Arduino.h>
#include <ESP8266WiFi.h>
#ifndef CONECCTION_H
#define CONECCTION_H
class WiFiConnection : public ESP8266WiFiClass
{
private:
    bool DHCP = false;

public:
    char *ssid;
    char *password;
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns;
    uint32_t connectionLastAttempt;
    WiFiConnection(char *ssid_, char *password_, IPAddress &ip_, IPAddress &gateway_, IPAddress &subnet_)
    {
        ssid = ssid_;
        password = password_;
        ip = ip_;
        gateway = gateway_;
        subnet = subnet_;
        dns = IPAddress(8, 8, 8, 8);
        connectionLastAttempt = 0;
    }
    WiFiConnection(char *ssid_, char *password_)
    {
        ssid = ssid_;
        password = password_;
        DHCP = true;
        connectionLastAttempt = 0;
    }
    bool connect()
    {
        bool result = false;
        // Inicializa el módulo wifi
        WiFi.mode(WIFI_STA); // Establece el módulo como estación wifi
        WiFi.disconnect();   // Se desconecta de cualquier WiFi conectado previamente
        Serial.println();

        // conecta con la red wifi
        Serial.print("Connecting to ");
        Serial.println(ssid);
        if (!DHCP)
            WiFi.config(ip, dns, gateway, subnet);
        WiFi.begin(ssid, password);
        int attemps = 0;
        while (WiFi.status() != WL_CONNECTED && attemps < 20) // intenta por 10 segundos si no sale
        {                                                     // Espera por una conexión WiFi
            delay(500);
            Serial.print(".");
            attemps++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            result = true;
        }
        else
        {
            Serial.println("WiFi not connected");
        }

        return result;
    }

    bool reconnectionHandler()
    {
        if (WiFi.status() != WL_CONNECTED && millis() - connectionLastAttempt > 10000) // cada 10 segundos intenta reconectarse
        {
            if (connectionLastAttempt == 0) // esto me indica que es la primera vez que entra. Entonces indico que se detectó pérdida de conexión
            {
                Serial.println("Connection lost!");
            }
            bool result = WiFiConnection::connect();
            connectionLastAttempt = millis();

            if (result)
            {
                Serial.println("Reconnected!");
                connectionLastAttempt = 0;
            }
            else
            {
                Serial.println("Reconnection Failed!");
            }
            return result;
        }
        return false;
    }
};
#endif