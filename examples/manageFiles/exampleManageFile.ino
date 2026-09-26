#include "FilesJr.h"
 
FilesJr files();
 
void setup() {
    if(files.isFileSystemMounted()) {
      Serial.println("File system mounted successfully");
    } else {
      Serial.println("Failed to mount file system");
  }
}