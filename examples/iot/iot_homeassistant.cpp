#include "serverweb.h"
#include "wifiJr.h"
#include "manageFiles2.h"
#include "IotJr2.h"
#include "Iothomeassistant.h"


// ----------------------- Functions for requering server web ------------------------------------
void read(ServerWebJr &server)
{
    File file = SPIFFS.open("/read.html", "r");
    if (!file)
    {
        server.getServer().send(404, "text/plain", "home.html nao encontrado");
        return;
    }

    String html = file.readString();
    file.close();
    server.getServer().send(200, "text/html", html);
}

void getSensors(ServerWebJr &server)
{

    delay(200);
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
        json);
}
// -------------------------------------------------------------------------------------------------------

ServerIoT iot(PIN_RESET, LED_BUILTIN);
HomeAssistant homeassistant();

void setup()
{

    // ------------------------------- This lines is need ----------------------------------------------
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_RESET, INPUT_PULLUP);
    // ---------------------------------------------------------------------------------------------------

    // ------------------ Create the server for connecting with wifi and server web
    Serial.begin(115200);
    delay(1500);
    Route routes[] = {
        {"/", read},
        {"/sensors", getSensors},
    };
    iot.initiated(routes, 2);

    // -----------------------------------------------------------------------------------------------------

    //------------------------- Create connection and configure the server MQTT
    homeassistant.setup();
    homeassistant.connectMQTT();

    //------------------------------------------------------------------------------------------------------

}

void loop()
{
    iot.loop_();
    delay(1000);

    //----------- This code is responsable for send data for homeassistant
    homeassistant.loop_();
    char data[50];
    float distance = 9;
    snprintf(buffer, sizeof(buffer), "%.2f", distance);
    homeassistant.sendData();
}
