 #include "serverweb.h"
  
ServerJr server(80, "yourSSID", "yourPassword");
  
 void setup() {
    server.addRoute("/", handleRoot);
    server.start("Server started on port 80");
}
  
void loop() {
    server.getServer().handleClient(); // Handle incoming client requests
    // Your main code here
}
  
 void handleRoot() {
     // Your main code here
    server.getServer().send(200, "text/plain", "Hello, World!");
}