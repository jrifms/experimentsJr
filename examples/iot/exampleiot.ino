#include "IotJr.h"
/*
* Muito importante para resetar o dispositivo você deve conectar o GND com o pino reset [definido conforme a linha a baixo]
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

void teste(ServerWebJr& server) {
        server.getServer().send(200, "text/plain", "Minha pagina esta ok !!!!! ");
        return;
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
        {"/test", teste},
    };
    modem.running(routes, 2);  // keep runing
    Serial.println(modem.msg);
    //-------------------------------------------------------------------------------------------------

}

void loop() {

// ------------------------ This line is responsable for keeping the server -------------------
    modem.server->getServer().handleClient();
    modem.errorConnectWifi();
// ---------------------------------------------------------------------------------------------------

    Serial.println("Running !");
    delay(500);
}