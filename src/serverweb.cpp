#include "serverweb.h"

ServerWebJr::ServerWebJr()
{
}


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

ServerWebJr::ServerWebJr(
    int port
)
    : port(port),
      server(port)
{
}



void ServerWebJr::addRoute(
    const char* path,
   RouteHandler handler
)
{
    server.on(path, [this, handler]() {
        handler(*this);
    });
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