#include "wifiJr.h"


WiFiJr::WiFiJr()
{
}

WiFiJr::WiFiJr(const char* ssid, const char* password): ssid(ssid), password(password) {
}

bool WiFiJr::connect() {

    WiFi.begin(ssid, password);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {

        if (millis() - startTime >= 15000) {
            return false;
        }

        delay(500);
    }

    return true;
}

bool WiFiJr::isConnected() {

    return WiFi.status() == WL_CONNECTED;
}

String WiFiJr::getIPAddress() {

    return WiFi.localIP().toString();
}

bool WiFiJr::createAccessPoint()
{
    delay(1000);
    WiFi.mode(WIFI_AP);
    return WiFi.softAP( ssid, password);
}

String WiFiJr::getAccessPointIP()
{
    return WiFi.softAPIP().toString();
}