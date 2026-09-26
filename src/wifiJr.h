/*
  wifiJr.h - Library for create wifi connection.
  Created by Junior Silva Souza, Setember 9, 2026.
  Released into the public domain.
*/

/**
 * @brief A simple wifi connection class for Arduino.
 * This class allows you to create a wifi connection that can handle network requests.
 * It uses the WiFi library to manage the wifi functionality.
 * @example
 *
 * #include "wifiJr.h"
 *
 * WiFiJr wifi("yourSSID", "yourPassword");
 *
 * void setup() {
 *   wifi.connect();
 * }
 *
 */
#ifndef WIFI_JR_H
#define WIFI_JR_H
#include <WiFi.h>

class WiFiJr {
  public:
    WiFiJr(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    String getIPAddress();

  private:
    const char* ssid;
    const char* password;
};