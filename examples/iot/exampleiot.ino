#include "IotJr.h"
/*
* Muito importante para resetar o dispositivo você deve conectar o GND com o pino reset [definido conforme a linha a baixo]
* Acesse: velhojack.local
*/
// ----------------------- Definition of object for create a modem -------------------------------
const int button_reset        = D1;
const int  led_information = LED_BUILTIN;
 IotJr modem(nameDefaultConf, button_reset, led_information);
// -------------------------------------------------------------------------------------------------------

// ----------------------- Functions for requering server web ------------------------------------
void read(ServerWebJr& server) {
	File file = SPIFFS.open("/read.html", "r");
    if (!file) {
        server.getServer().send(404,"text/plain","home.html nao encontrado");
        return;
    }

    String html = file.readString();
    file.close();
    server.getServer().send(200, "text/html", html);
}

void getSensors(ServerWebJr& server) {

    float temperatura = 24.6;
    float umidade = 58;
    float pressao = 1013;

    String json = "{";
    json += "\"sensores\":[";

    json += "{";
    json += "\"nome\":\"Temperatura\",";
    json += "\"valor\":" + String(temperatura, 1) + ",";
    json += "\"unidade\":\"°C\"";
    json += "},";

    json += "{";
    json += "\"nome\":\"Umidade\",";
    json += "\"valor\":" + String(umidade, 0) + ",";
    json += "\"unidade\":\"%\"";
    json += "},";

    json += "{";
    json += "\"nome\":\"Pressão\",";
    json += "\"valor\":" + String(pressao, 0) + ",";
    json += "\"unidade\":\"hPa\"";
    json += "}";

    json += "]";
    json += "}";

    server.getServer().send(
        200,
        "application/json",
        json
    );
}
// -------------------------------------------------------------------------------------------------------

void setup() {

    // ------------------------------- This lines is need ----------------------------------------------
    pinMode(led_information, OUTPUT);
    pinMode(button_reset, INPUT_PULLUP);
    // ---------------------------------------------------------------------------------------------------

    Serial.begin(115200);
    
    //--------------------------- Create modem --------------------------------------------------
    Serial.println(modem.msg);
    modem.initiated(); // Create config if not exist
    Serial.println(modem.msg);
    Route routes[] = {
        {"/", read},
        {"/sensors", getSensors},                        //10.9.35.242
    };
    modem.running(routes, 2);  // keep runing
    Serial.println(modem.msg);
    //-------------------------------------------------------------------------------------------------

}

void loop() {

// ------------------------ This line is responsable for keeping the server -------------------
   if(modem.typeNetWork == "accesspoint"){
        modem.wifi.handleDNS();
   }else{
        modem.wifi.handleMDNS();
   }
    modem.server->getServer().handleClient();
    modem.errorConnectWifi();
// ---------------------------------------------------------------------------------------------------

    Serial.println("Running !");
    delay(500);
}