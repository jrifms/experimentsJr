#include "wifiJr.h"

WiFiJr::WiFiJr(const char* ssid, const char* password) : ssid(ssid), password(password) {}

void WiFiJr::connect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Trying to connect to WiFi...");
  }
}

bool WiFiJr::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

String WiFiJr::getIPAddress() {
  return WiFi.localIP().toString();
}