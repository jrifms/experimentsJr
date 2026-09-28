/*
  ServerWebJr.h - Library for create web server.
  Created by Junior Silva Souza, September 9, 2026.
  Released into the public domain.
*/

/**
 * @brief A simple web server class for Arduino.
 *
 * This class allows you to create a web server that can handle
 * HTTP requests and serve web pages.
 *
 * It supports ESP8266 and ESP32 platforms.
 */

#ifndef SERVER_WEB_JR_H
#define SERVER_WEB_JR_H

#if defined(ESP8266)

    #include <ESP8266WebServer.h>
    using WebServer = ESP8266WebServer;

#elif defined(ESP32)

    #include <WebServer.h>
    using WebServer = WebServer;

#else

    #error "Plataforma não suportada"

#endif


class ServerWebJr {

  public:

    ServerWebJr(
      int port,
      const char* ssid,
      const char* password
    );

    ServerWebJr(
      int port
    );

    void addRoute(
      const char* path,
      void (*callback)()
    );

    void start(const char* msg = nullptr);

    void handleClient();

    WebServer & getServer();

  private:

    int port;

    const char* ssid;
    const char* password;

    WebServer server;
};

#endif