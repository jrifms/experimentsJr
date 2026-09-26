/*
  ServerWebJr.h - Library for create server web.
  Created by Junior Silva Souza, Setember 9, 2026.
  Released into the public domain.
*/

/**
 * @brief A simple web server class for Arduino.
 * This class allows you to create a web server that can handle HTTP requests and serve web pages.
 * It uses the WebServer library to manage the server functionality.
 * @example
 * 
 * #include "serverweb.h"
 * 
 * ServerJr server(80, "yourSSID", "yourPassword");
 * 
 * void setup() {
 *   server.addRoute("/", handleRoot);
 *   server.start("Server started on port 80");
 * }
 * 
 * void loop() {
 *   server.getServer().handleClient(); // Handle incoming client requests
 *   // Your main code here
 * }
 * 
 * void handleRoot() {
 *    // Your main code here
 *   server.getServer().send(200, "text/plain", "Hello, World!");
 * }
 * 
 */
#ifndef SERVER_JR_H
#define SERVER_JR_H

#include<WebServer.h>

class ServerJr{

  public:
    ServerJr(const int port=80, const char* ssid, const char* password);
    void addRoute(const char* path, void (*callback)());
    void start(char *msg=nullptr);
    WebServer& getServer() { return server; } // Getter for the server object

  private:
    int port;
    const char* ssid;
    const char* password;
    WebServer server;
};
#endif