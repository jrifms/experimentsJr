#include "Iothomeassistant.h"


HomeAssistant::HomeAssistant()
{
    this.mqtt.setClient(this.espClient);
    if(DEBUG_MSG){
        Serial.println("[ - Begin the connection with mqtt");
    }
}

void HomeAssistant::setup()
{
    this.mqtt.setServer(MQTT_SERVER, PORT_MQTT);
}

// Conectar MQTT
void HomeAssistant::connectMQTT()
{
    while (!this.mqtt.connected())
    {
        if (this.mqtt.connect(NAME_DEVICE_HOMEASSISTANT, MQTT_USER, MQTT_PASSWORD))
        {
            if(DEBUG_MSG){
                Serial.println("MQTT conectado!");
            }
        }
        else
        {
            if(DEBUG_MSG){
                Serial.println("Falha MQTT!");
                delay(3000);
            }
        }
    }
}

//This code is responsable for putting in loop for running this project
void HomeAssistant::loop_()
{
    if(!this.mqtt.connected()){
        this->connectMQTT();
    }else{
        if(DEBUG_MSG){
            Serial.println("[ - Problems in connection mqtt!");
        }
    }

    this.mqtt.loop();
}

//This code is responsable for send data
void HomeAssistant::sendData(chat buffer)
{
    mqtt.publish(NAME_DEVICE_SEND_DATA, buffer);
}

 