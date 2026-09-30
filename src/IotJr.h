#ifndef IOTJR_H
#define IOTJR_H

#include <Arduino.h>

#include "manageFiles.h"
#include "serverweb.h"
#include "wifiJr.h"


// ============================================================
// Configuração padrão
// ============================================================

extern const String nameFirstSSID;
extern const String passwordDefault;
extern const String typeNetWorkDefault;
extern const String nameDefaultConf;
extern const String nameDNS;

// ============================================================
// Rotas
// ============================================================

typedef void (*RouteFunction)(ServerWebJr&);

struct Route {
    const char* path;
    RouteFunction function;
};


// ============================================================
// Funções de página
// ============================================================

void home(ServerWebJr& server);
void read(ServerWebJr& server);
void chapa(ServerWebJr& server);
void configure(ServerWebJr& server);


// ============================================================
// Classe IotJr
// ============================================================

class IotJr {

public:

    String msg;

    bool setupFactory = true;
    ServerWebJr* server = nullptr;


    IotJr(
        const String& conf_file,
        int buttonReset,
        int ledInformation
    );


    void initiated();


    void running(
        Route routes[],
        int quantidade
    );


    void errorConnectWifi();


    void ledStartingConnectWifi();


    void setCredentialsNet(
        String typeNetWork,
        String SSID,
        String password
    );


    void extractingData(
        String dados
    );


    bool mountingSystemFiles();


    void createConfigFactory();


    bool isFirstSetup();


    void checkPinAnd_Reset();


private:

    FilesJr files;
    String conf;
    String typeNetWork;
    String SSID;
    String password;
     WiFiJr wifi;
    bool isMounted = false;
    bool isConnected = false;
    int buttonReset;
    int ledInformation;
};

#endif