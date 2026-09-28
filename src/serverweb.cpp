#include "ServerWebJr.h"


ServerJr::ServerJr(
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


void ServerJr::addRoute(
    const char* path,
    void (*callback)()
)
{
    server.on(path, callback);
}


void ServerJr::start(const char* msg)
{
    server.begin();

    if (msg != nullptr) {
        Serial.println(msg);
    }
}


void ServerJr::handleClient()
{
    server.handleClient();
}


WebServerJr& ServerJr::getServer()
{
    return server;
}