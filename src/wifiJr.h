/*
  wifiJr.h - Library for create wifi connection.
  Created by Junior Silva Souza, September 9, 2026.
  Released into the public domain.
*/

/**
 * @brief A simple WiFi connection class for Arduino.
 */

#ifndef WIFI_JR_H
#define WIFI_JR_H

#if defined(ESP8266)

    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>

#elif defined(ESP32)

    #include <WiFi.h>
    #include <ESPmDNS.h>

#else

    #error "Placa não suportada"

#endif

#include <DNSServer.h>

extern DNSServer dnsServer;

class WiFiJr {

  public:

    WiFiJr();

    WiFiJr(const char* ssid, const char* password);


    bool connect();

    bool isConnected();

    bool createAccessPoint();

    String getAccessPointIP();

    String getIPAddress();

    void startDNS(const char* domain); // For create a dns
    
    void handleDNS();

    // mDNS para rede existente
    bool startMDNS(const char* hostname);

    void handleMDNS();

  private:

    const char* ssid;
    const char* password;
};

#endif