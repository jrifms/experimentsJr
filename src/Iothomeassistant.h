#ifndef HOMEASSISTANT_MQTT_H
#define HOMEASSISTANT_MQTT_H

#include <PubSubClient.h>

#define MQTT_SERVER "192.168.1.100";
#define MQTT_USER "esp32";
#define MQTT_PASSWORD "SENHA";
#define NAME_DEVICE_HOMEASSISTANT "ESP32_ULTRASONIC_IFMS_JD"
#define NAME_DEVICE_SEND_DATA "casa/esp32/distancia"
#define PORT_MQTT 1883

class HomeAssistant
{

public:
    
    HomeAssistant();
    void setup();
    void connectMQTT();
    void loop_();
    void sendData(chat buffer[50]);

private:
    WiFiClient espClient;
    PubSubClient mqtt;
};

#endif