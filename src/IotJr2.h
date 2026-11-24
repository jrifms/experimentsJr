#ifndef IOTJR2_H
#define IOTJR2_H

#include "serverweb.h"
#include "wifiJr.h"
#include "manageFiles2.h"



// ============================================================
// Rotas
// ============================================================
typedef void (*RouteFunction)(ServerWebJr &);
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

class ServerIoT
{

public:
    FileOperation file;
    WiFiJr wifi;
    ServerWebJr *server = nullptr;
    int buttonReset, ledInformation;
    bool firstTime = false;


    ServerIoT(int buttonReset, int ledInformation);
    void initiated(Route routes[], int quantidade);
    void checkPinAndReset();
    void createConfigFactory();
    void ledStartingConnectWifi(int time_wait);
    void loop_();

private:
};

#endif