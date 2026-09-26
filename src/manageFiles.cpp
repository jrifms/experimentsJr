#include "manageFiles.h"

FilesJr::FilesJr() {    
    // Initialize the file system (SPIFFS)
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
        this->filesytem = false;
    } else {
        Serial.println("SPIFFS mounted successfully");
        this->filesytem = true;
    }
}
 
FilesJr::deleteFile(const char* path) {
    if (SPIFFS.exists(path)) {
        SPIFFS.remove(path);
        return true;
    }
    return false;
}

FilesJr::createFile(const char* content) {
    File file = SPIFFS.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to create file");
        return false;
    }
    file.print(content);
    file.close();
    return true;
}

FilesJr::readFile(String& content) {
    File file = SPIFFS.open(path, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }
    content = file.readString();
    file.close();
    return true;
}

FilesJr::listFiles() {
    Serial.println("Listing files:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.print("FILE: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
}

FilesJr::isFileSystemMounted() {
    return this->filesytem;
}

FilesJr::fileExists(const char* path) {
    return SPIFFS.exists(path);
}