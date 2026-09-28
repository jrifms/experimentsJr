#include "serverweb.h"
#include "wifiJr.h"

ServerWebJr server(80, "SSID", "PASSWORD");
WiFiJr wifi("SSID", "PASSWORD");

void handleRoot() {
	server.getServer().send(200, "text/plain", "Ola!");
}

void setup() {
	Serial.begin(115200);
	if (wifi.connect()) {

        Serial.println("WiFi conectado!");
        Serial.println(wifi.getIPAddress());

    } else {

        Serial.println("Não foi possível conectar ao WiFi.");

    }

	server.addRoute("/", handleRoot);
	server.start("Servidor iniciado");
}

void loop() {
	server.getServer().handleClient();
	Serial.println(wifi.getIPAddress());
	delay(5000);
}