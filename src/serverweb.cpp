#include "serverweb.h"

ServerJr::ServerJr(const int port=80, char ssid, char password): server(port){
  // Initialize the member variables with the provided values
  this->port = port;
  this->ssid = ssid;
  this->password = password;
  //Create a WebServer object with the specified port
  this->server = WebServer(port);
}


ServerJr::addRoute(const char* path, void (*callback)()) {
  // Add a route to the server with the specified path and callback function
  server.on(path, callback);
}

void ServerJr::start(char *msg=nullptr) {
  server.begin();
  if (msg) {
    Serial.println(msg);
  }
}