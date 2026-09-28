
#include "wifiJr.h"

WiFiJr wifi("SSID", "PASSWORD");

void setup() {

    Serial.begin(115200);

    if (wifi.connect()) {

        Serial.println("WiFi conectado!");
        Serial.println(wifi.getIPAddress());

    } else {

        Serial.println("Não foi possível conectar ao WiFi.");

    }
}

void loop() {
	Serial.println(wifi.getIPAddress());
	delay(5000);
}