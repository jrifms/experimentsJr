#include "serverweb.h"
#include "wifiJr.h"

WiFiJr wifi("Teste_Rede_JR", "12345678");
ServerWebJr server(80);


void handleRoot() {
	server.getServer().send(200, "text/plain", "Ola!");
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);

	for(int i=0; i< 5; i++){
		digitalWrite(LED_BUILTIN, LOW);
  	delay(1000);

  	digitalWrite(LED_BUILTIN, HIGH);
  	delay(1000);
	}


	Serial.println("Iniciando conexao !");
	if (wifi.createAccessPoint()) {

        Serial.println("WiFi conectado!");
        Serial.println(wifi.getAccessPointIP());

    } else {

        Serial.println("Não foi possível conectar ao WiFi.");

    }

	server.addRoute("/", handleRoot);
	server.start("Servidor iniciado");
}

void loop() {
	server.getServer().handleClient();
	//Serial.println(wifi.getAccessPointIP());
	//delay(5000);
}