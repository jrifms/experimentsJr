#include "serverweb.h"


ServerWebJr::ServerWebJr(
    int port,
    const char* ssid,
    const char* password
)
    : port(port),
      ssid(ssid),
      password(password),
      server(port)
{
}


void ServerWebJr::addRoute(
    const char* path,
    void (*callback)()
)
{
    server.on(path, callback);
}


void ServerWebJr::start(const char* msg)
{
    server.begin();

    if (msg != nullptr) {
        Serial.println(msg);
    }
}


void ServerWebJr::handleClient()
{
    server.handleClient();
}


WebServer& ServerWebJr::getServer()
{
    return server;
}