#include "wifiJr.h"
DNSServer dnsServer;

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
    WiFi.mode(WIFI_AP);
    return WiFi.softAP( ssid, password);
}

String WiFiJr::getAccessPointIP()
{
    return WiFi.softAPIP().toString();
}

void WiFiJr::startDNS(const char* domain)
{
    Serial.println("Iniciando DNS...");

    Serial.print("Dominio: ");
    Serial.println(domain);

    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());

    bool result = dnsServer.start(
        53,
        domain,
        WiFi.softAPIP()
    );

    Serial.print("DNS iniciado: ");
    Serial.println(result ? "SIM" : "NAO");
}

void WiFiJr::handleDNS()
{
    dnsServer.processNextRequest();
}