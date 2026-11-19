#ifndef CONSTANTS_H
#define CONSTANTS_H

//Definição de SSID e senha da rede Wi-Fi
#ifndef SSID_LOCAL
    #define SSID_LOCAL "Teste_Rede_JR"
#endif

#ifndef PASSWORD_LOCAL
    #define PASSWORD_LOCAL "12345678"
#endif

// Definição de pinos e constantes
#ifndef LED_BUILTIN
    #if defined(ESP32)
        #define LED_BUILTIN 2
    #endif
#endif

// Definição da lógica de acionamento
#if defined(ESP8266)
    #define LED_ON  LOW
    #define LED_OFF HIGH
#elif defined(ESP32)
    #define LED_ON  HIGH
    #define LED_OFF LOW
#endif

#endif