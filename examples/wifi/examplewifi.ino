#include "wifiJr.h"
 
 WiFiJr wifi("yourSSID", "yourPassword");
 
 void setup() {
     wifi.connect();
 }
 